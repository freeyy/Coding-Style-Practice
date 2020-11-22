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

// Ex 28.2
func lines(path string, chLine chan<- string) {
	data, _ := ioutil.ReadFile(path)
	lines := strings.Split(strings.ToLower(string(data)), "\n")
	for _, line := range lines {
		chLine <- line
	}
	close(chLine)
}

func allWords(chLine <-chan string, chWord chan<- string) {
	for line := range chLine {
		reg := regexp.MustCompile("[a-z]{2,}")
		words := reg.FindAllString(line, -1)
		for _, word := range words {
			chWord <- word
		}
	}
	close(chWord)
}

func nonStopWords(chWord <-chan string, chNonStop chan<- string) {
	pathStop := "../stop_words.txt"
	data, _ := ioutil.ReadFile(pathStop)

	stopWrods := strings.Split(string(data), ",")
	stops := make(map[string]bool)
	for _, word := range stopWrods {
		stops[word] = true
	}

	for word := range chWord {
		if !stops[word] {
			chNonStop <- word
		}
	}
	close(chNonStop)
}

func countSort(chNonStop <-chan string, out chan<- []keyValue) {
	freqs := make(map[string]int)
	for word := range chNonStop {
		freqs[word]++
	}

	var ss []keyValue
	for k, v := range freqs {
		ss = append(ss, keyValue{k, v})
	}

	sort.Slice(ss, func(i, j int) bool {
		return ss[i].value > ss[j].value
	})

	out <- ss
	close(out)
}

func main() {
	// In Go, streaming style is implemented using channels
	// similar to Chapter 29 actually
	chLine := make(chan string, 10)
	chWord := make(chan string, 10)
	chNonStop := make(chan string, 10)
	chOutput := make(chan []keyValue)

	go lines(os.Args[1], chLine)
	go allWords(chLine, chWord)
	go nonStopWords(chWord, chNonStop)
	go countSort(chNonStop, chOutput)

	res := <-chOutput
	for i := 0; i < 25; i++ {
		fmt.Println(res[i].key, " - ", res[i].value)
	}
}
