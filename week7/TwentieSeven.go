package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"regexp"
	"sort"
	"strings"
)

type keyValue struct {
	key   string
	value int
}

type column interface {
	update()
}

// COLUMN: ALL_WORDS
type allWordsClm struct {
	Words     []string
	UpdatePtr int
}

// COLUMN: ALL_STOPS
type allStopsClm struct {
	Stops map[string]bool
}

// COLUMN: NON_STOP_WORDS
type noneStopsClm struct {
	Words     []string
	UpdatePtr int
}

// COLUMN: COUNTS
type countsClm struct {
	Freqs map[string]int
}

// COLUMN: SORTED_DATA
type sortedDataClm struct {
	Kv []keyValue
}

var curInput []string

func (clm *allWordsClm) init(path string) {
	data, _ := ioutil.ReadFile(path)
	reg := regexp.MustCompile("[a-z]{2,}")
	curInput = reg.FindAllString(strings.ToLower(string(data)), -1)
}

func (clm *allStopsClm) init() {
	clm.Stops = make(map[string]bool)
	pathStop := "../stop_words.txt"
	data, _ := ioutil.ReadFile(pathStop)

	stopWrods := strings.Split(string(data), ",")
	for _, word := range stopWrods {
		clm.Stops[word] = true
	}
}

func (clm *allWordsClm) update() {
	clm.UpdatePtr = len(clm.Words) // delta update
	clm.Words = append(clm.Words, curInput...)
}

func (clm *allStopsClm) update() {
}

func (clm *noneStopsClm) update() {
	clm.UpdatePtr = len(clm.Words) // delta update
	words := allWords.Words
	stops := allStops.Stops

	for i := allWords.UpdatePtr; i < len(words); i++ {
		if !stops[words[i]] {
			clm.Words = append(clm.Words, words[i])
		}
	}
}

func (clm *countsClm) update() {
	words := nonStops.Words
	for i := nonStops.UpdatePtr; i < len(words); i++ {
		clm.Freqs[words[i]]++
	}
}

func (clm *sortedDataClm) update() {
	clm.Kv = clm.Kv[:0]
	freqs := counts.Freqs
	for k, v := range freqs {
		clm.Kv = append(clm.Kv, keyValue{k, v})
	}
	sort.Slice(clm.Kv, func(i, j int) bool {
		return clm.Kv[i].value > clm.Kv[j].value
	})
}

// declare all columns
var allWords = allWordsClm{}
var allStops = allStopsClm{}
var nonStops = noneStopsClm{}
var counts = countsClm{make(map[string]int)}
var sorted = sortedDataClm{}

var clms = []column{&allWords, &allStops, &nonStops, &counts, &sorted}

// update all columns on new files
func update() {
	for _, clm := range clms {
		clm.update()
	}
}

func print() {
	for i := 0; i < 25; i++ {
		fmt.Println(sorted.Kv[i].key, " - ", sorted.Kv[i].value)
	}
}

func main() {
	allWords.init(os.Args[1])
	allStops.init()
	update()
	print()

  for {
    // Ex 27.2
    fmt.Println("\nPlease enter another file name:")
    fmt.Println("e.g. additional.txt")

    var newPath string
    fmt.Scanln(&newPath)
    allWords.init(newPath)
    update()
    print()
  }
}
