package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"reflect"
	"regexp"
	"sort"
	"strings"
	"sync"
)

type message struct {
	name string
	obj  interface{}
}

type activeWF interface {
	dispatch([]message) // dispatch a msg to itself
}

func send(obj interface{}, msg []message) {
	robj := reflect.ValueOf(obj).Elem()
	// fmt.Println(robj.Type(), "\nsend: ", msg)
	rch := robj.Field(0)
	rch.Send(reflect.ValueOf(msg))
}

type dataStorageManager struct {
	Ch          chan []message
	data        string
	pathToFile  string
	stopWordMgr *stopWordManager
}

func newDSM() *dataStorageManager {
	var dsm dataStorageManager
	dsm.Ch = make(chan []message)
	return &dsm
}

func (dsm *dataStorageManager) dispatch(msg []message) {
	switch msg[0].name {
	case "init":
		dsm.init(msg[1:])
	case "send_word_freqs":
		dsm.processWords(msg[1:])
	default: // forward
		send(dsm.stopWordMgr, msg)
	}
}

func (dsm *dataStorageManager) init(msg []message) {
	dsm.pathToFile = msg[0].name
	dsm.stopWordMgr = msg[1].obj.(*stopWordManager)

	data, _ := ioutil.ReadFile(dsm.pathToFile)
	dsm.data = strings.ToLower(string(data))
}

func (dsm *dataStorageManager) processWords(msg []message) {
	recipient := msg[0]
	reg := regexp.MustCompile("[a-z]{2,}")
	words := reg.FindAllString(dsm.data, -1)

	for _, word := range words {
		send(dsm.stopWordMgr, []message{{"filter", nil}, {word, nil}})
	}
	send(dsm.stopWordMgr, []message{{"top25", nil}, recipient})
}

type stopWordManager struct {
	Ch          chan []message
	stops       map[string]bool
	wordFreqMgr *wordFrequencyManager
}

func newSWM() *stopWordManager {
	var swm stopWordManager
	swm.Ch = make(chan []message)
	swm.stops = make(map[string]bool)
	return &swm
}

func (swm *stopWordManager) dispatch(msg []message) {
	switch msg[0].name {
	case "init":
		swm.init(msg[1:])
	case "filter":
		swm.filter(msg[1:])
	default:
		send(swm.wordFreqMgr, msg)
	}
}

func (swm *stopWordManager) init(msg []message) {
	swm.wordFreqMgr = msg[0].obj.(*wordFrequencyManager)

	pathStop := "../stop_words.txt"
	data, _ := ioutil.ReadFile(pathStop)

	stopWrods := strings.Split(string(data), ",")
	for _, word := range stopWrods {
		swm.stops[word] = true
	}
}

func (swm *stopWordManager) filter(msg []message) {
	word := msg[0].name

	if !swm.stops[word] {
		send(swm.wordFreqMgr, []message{{"word", nil}, {word, nil}})
	}
}

type wordFrequencyManager struct {
	Ch    chan []message
	freqs map[string]int
}

type kv struct {
	Key   string
	Value int
}

func newWFM() *wordFrequencyManager {
	var wfm wordFrequencyManager
	wfm.Ch = make(chan []message)
	wfm.freqs = make(map[string]int)
	return &wfm
}

func (wfm *wordFrequencyManager) dispatch(msg []message) {
	switch msg[0].name {
	case "word":
		wfm.incrementCount(msg[1:])
	case "top25":
		wfm.top25(msg[1:])
	}
}

func (wfm *wordFrequencyManager) incrementCount(msg []message) {
	word := msg[0].name
	wfm.freqs[word]++
}

func (wfm *wordFrequencyManager) top25(msg []message) {
	recipient := msg[0].obj

	var ss []kv
	for k, v := range wfm.freqs {
		ss = append(ss, kv{k, v})
	}

	sort.Slice(ss, func(i, j int) bool {
		return ss[i].Value > ss[j].Value
	})

	send(recipient, []message{{"top25", nil}, {"sorted_kv", ss}})
}

type wordFrequencyController struct {
	Ch  chan []message
	dsm *dataStorageManager
}

func newWFC() *wordFrequencyController {
	var wfc wordFrequencyController
	wfc.Ch = make(chan []message)
	return &wfc
}

func (wfc *wordFrequencyController) dispatch(msg []message) {
	switch msg[0].name {
	case "run":
		wfc.run(msg[1:])
	case "top25":
		wfc.display(msg[1:])
	default:
		panic(fmt.Sprintf("[%v]Unkown Msg: %v", reflect.TypeOf(wfc), msg))
	}
}

func (wfc *wordFrequencyController) run(msg []message) {
	wfc.dsm = msg[0].obj.(*dataStorageManager)
	send(wfc.dsm, []message{{"send_word_freqs", nil}, {"wfc", wfc}})
}

func (wfc *wordFrequencyController) display(msg []message) {
	freqs := msg[0].obj.([]kv)
	for _, kv := range freqs[:25] {
		fmt.Println(kv.Key, " - ", kv.Value)
	}
	fmt.Println("")
	close(wfc.Ch)

	send(wfc.dsm, []message{{"die", nil}})
}

// run every routine
func run(wf activeWF, wg *sync.WaitGroup) {
	obj := reflect.ValueOf(wf).Elem()
	ch := obj.Field(0).Interface().(chan []message)

	defer wg.Done()
	for msg := range ch {
		wf.dispatch(msg)
		if msg[0].name == "die" {
			// fmt.Println(reflect.TypeOf(wf), "die")
			close(ch)
		}
	}
}

func main() {
	var wg sync.WaitGroup
	wg.Add(4)

	wordFreqMgr := newWFM()
	go run(wordFreqMgr, &wg)

	stopWordMgr := newSWM()
	go run(stopWordMgr, &wg)
	send(stopWordMgr, []message{{"init", nil}, {"wfm", wordFreqMgr}})

	dataMgr := newDSM()
	go run(dataMgr, &wg)
	send(dataMgr, []message{{"init", nil}, {os.Args[1], nil}, {"swm", stopWordMgr}})

	wordFreqCtl := newWFC()
	go run(wordFreqCtl, &wg)
	send(wordFreqCtl, []message{{"run", nil}, {"dsm", dataMgr}})

	wg.Wait()
}
