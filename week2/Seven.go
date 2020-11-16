package main

import (
	"fmt"
	"io/ioutil"
	"regexp"
	"sort"
	"strings"
)

func main() {
	path := "../pride-and-prejudice.txt"
	data, _ := ioutil.ReadFile(path)

	reg := regexp.MustCompile("[a-z]{2,}")
	words := reg.FindAllString(strings.ToLower(string(data)), -1)

	pathStop := "../stop_words.txt"
	data, _ = ioutil.ReadFile(pathStop)

	stopWrods := strings.Split(string(data), ",")
	stops := make(map[string]bool)
	for _, word := range stopWrods {
		stops[word] = true
	}

	freqs := make(map[string]int)
	for _, word := range words {
		if !stops[word] {
			freqs[word]++
		}
	}

	type kv struct {
		Key   string
		Value int
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
