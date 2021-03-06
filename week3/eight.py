#!/usr/bin/env python
from operator import index
import re, sys, operator

# Mileage may vary. If this crashes, make it lower
RECURSION_LIMIT = 5000
# We add a few more, because, contrary to the name,
# this doesn't just rule recursion: it rules the 
# depth of the call stack
sys.setrecursionlimit(RECURSION_LIMIT+10)

def count(word_list, stopwords, wordfreqs):
    # What to do with an empty list
    if word_list == []:
        return
    # The inductive case, what to do with a list of words
    else:
        # Process the head word
        word = word_list[0]
        if word not in stopwords:
            if word in wordfreqs:
                wordfreqs[word] += 1
            else:
                wordfreqs[word] = 1
        # Process the tail 
        count(word_list[1:], stopwords, wordfreqs)

def wf_print(wordfreq):
    if wordfreq == []:
        return
    else:
        (w, c) = wordfreq[0]
        print(w, '-', c)
        wf_print(wordfreq[1:])

# get the top 25
def recursive_sort(items, depth):
    if not items or depth == 0:
        return []
    max_val = max(items, key=operator.itemgetter(1))
    del items[items.index(max_val)]
    res = recursive_sort(items, depth - 1)
    res.append(max_val)
    return res



stop_words = set(open('../stop_words.txt').read().split(','))
words = re.findall('[a-z]{2,}', open(sys.argv[1]).read().lower())
word_freqs = {}
# Theoretically, we would just call count(words, stop_words, word_freqs)
# Try doing that and see what happens.
for i in range(0, len(words), RECURSION_LIMIT):
    count(words[i:i+RECURSION_LIMIT], stop_words, word_freqs)

res = recursive_sort(list(word_freqs.items()), 25)
res.reverse()
wf_print(res)
