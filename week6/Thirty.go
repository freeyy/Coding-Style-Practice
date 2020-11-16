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

type kv struct {
	Key   string
	Value int
}

func main() {
	chWords := make(chan string, 10)
	chFreqs := make(chan map[string]int, 10)

	// load input words
	path := os.Args[1]
	data, _ := ioutil.ReadFile(path)
	reg := regexp.MustCompile("[a-z]{2,}")
	words := reg.FindAllString(strings.ToLower(string(data)), -1)

	// load stop words
	pathStop := "../stop_words.txt"
	stops := make(map[string]bool)
	data, _ = ioutil.ReadFile(pathStop)
	stopWrods := strings.Split(string(data), ",")
	for _, word := range stopWrods {
		stops[word] = true
	}

	// push words
	go func() {
		defer close(chWords)
		for _, word := range words {
			chWords <- word
		}
	}()

	// start 5 go routines
	var wg sync.WaitGroup
	wg.Add(5)
	for i := 0; i < 5; i++ {
		go func() {
			defer wg.Done()
			freqs := make(map[string]int)
			for word := range chWords {
				if !stops[word] {
					freqs[word]++
				}
			}
			chFreqs <- freqs
		}()
	}

	go func() {
		wg.Wait()
		close(chFreqs)
	}()

	// main
	freqs := make(map[string]int)
	for freq := range chFreqs {
		for k, v := range freq {
			freqs[k] += v
		}
	}
	var ss []kv
	for k, v := range freqs {
		ss = append(ss, kv{k, v})
	}

	sort.Slice(ss, func(i, j int) bool {
		return ss[i].Value > ss[j].Value
	})

	for i := 0; i < 25; i++ {
		fmt.Println(ss[i].Key, " - ", ss[i].Value)
	}
}
