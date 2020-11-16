package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"regexp"
	"sort"
	"strings"
	"sync"
)

const nMapWorker int = 10
const nRdcWorker int = 5

type kv struct {
	key   string
	value int
}

var stops map[string]bool

// output:  ["aa bb cc dd ..."]		>- channel[text]
func reader(out [nMapWorker]chan string, path string) {
	// read data
	data, _ := ioutil.ReadFile(path)
	lines := strings.Split(string(data), "\n")

	// simulate batch input
	var batches [nMapWorker][]string
	for id, line := range lines {
		i := id % nMapWorker
		batches[i] = append(batches[i], line)
	}

	// dispatch
	for i := range out {
		go func(ch chan<- string, lines []string) {
			defer close(ch)
			for _, line := range lines {
				ch <- line
			}
		}(out[i], batches[i])
	}
}

// input:  ["aa bb cc dd ..."]				<- channel[input]
// output: [[("aa", 1), ("bb", 1), ...]]	-> channel[map]
func mapper(in <-chan string, out chan<- []kv) {
	var ss []kv

	for line := range in {
		reg := regexp.MustCompile("[a-z]{2,}")
		matches := reg.FindAllString(strings.ToLower(line), -1)
		for _, word := range matches {
			if !stops[word] {
				ss = append(ss, kv{word, 1})
			}
		}
	}

	out <- ss
	close(out)
}

// input:  [[("aa", 1), ("zz", 1), ...]]		<- []channel[map]
// output: [("aa", 1), ("bb", 1), ...]			-> []channel[reducer]
func shuffle(ins [nMapWorker]chan []kv, outs [nRdcWorker]chan kv) {
	var wg sync.WaitGroup
	wg.Add(len(ins))

	for _, in := range ins {
		go func(ch <-chan []kv) {
			for kvs := range ch {
				for _, kv := range kvs {
					// into 5 groups
					id := int(kv.key[0]-'a') / 5
					if kv.key[0] == 'z' {
						id = 4
					}
					outs[id] <- kv
				}
			}
			wg.Done()
		}(in)
	}

	wg.Wait()
	for _, ch := range outs {
		close(ch)
	}
}

// input:  [("aa", 1), ("bb", 1), ...]		<- []channel[reducer]
// output: [[("aa", 2), ("bb", 5), ...]]	-> []channel[output]
func reducer(in <-chan kv, out chan<- []kv) {
	freqs := make(map[string]int)
	for kv := range in {
		freqs[kv.key]++
	}
	var ss []kv
	for k, v := range freqs {
		ss = append(ss, kv{k, v})
	}
	out <- ss
	close(out)
}

func collector(in <-chan []kv, res *[]kv, mu *sync.Mutex, wg *sync.WaitGroup) {
	for kvs := range in {
		mu.Lock()
		*res = append(*res, kvs...)
		mu.Unlock()
	}
	wg.Done()
}

func loadStopWords() {
	pathStop := "../stop_words.txt"
	data, _ := ioutil.ReadFile(pathStop)

	stopWrods := strings.Split(string(data), ",")
	stops = make(map[string]bool)
	for _, word := range stopWrods {
		stops[word] = true
	}
}

func main() {
	bufSize := 10
	var chTxt [nMapWorker]chan string
	var chMap [nMapWorker]chan []kv
	var chRdc [nRdcWorker]chan kv
	var chPrt [nRdcWorker]chan []kv

	var res []kv
	var mu sync.Mutex
	var wg sync.WaitGroup
	wg.Add(nRdcWorker)
	loadStopWords()

	// init channels
	for i := 0; i < nMapWorker; i++ {
		chTxt[i] = make(chan string, bufSize)
		chMap[i] = make(chan []kv, bufSize)
	}

	for i := 0; i < nRdcWorker; i++ {
		chRdc[i] = make(chan kv, bufSize)
		chPrt[i] = make(chan []kv)
	}

	// run map-reduce
	go reader(chTxt, os.Args[1])

	for i := 0; i < nMapWorker; i++ {
		go mapper(chTxt[i], chMap[i])
	}

	go shuffle(chMap, chRdc)

	for i := 0; i < nRdcWorker; i++ {
		go reducer(chRdc[i], chPrt[i])
		go collector(chPrt[i], &res, &mu, &wg)
	}

	// print result
	wg.Wait()
	sort.Slice(res, func(i, j int) bool {
		return res[i].value > res[j].value
	})
	for i := 0; i < 25; i++ {
		fmt.Println(res[i].key, " - ", res[i].value)
	}
}
