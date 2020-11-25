import sys, string
import numpy as np

"""
Based on tf-03.py
"""

# Example input: "Hello  World!" 
characters = np.array([' ']+list(open(sys.argv[1], encoding='UTF-8-sig').read())+[' '])
# Result: array([' ', 'H', 'e', 'l', 'l', 'o', ' ', ' ', 
#           'W', 'o', 'r', 'l', 'd', '!', ' '], dtype='<U1')


# Normalize
characters[~np.char.isalpha(characters)] = ' '
characters = np.char.lower(characters)
# Result: array([' ', 'h', 'e', 'l', 'l', 'o', ' ', ' ', 
#           'w', 'o', 'r', 'l', 'd', ' ', ' '], dtype='<U1')


### Split the words by finding the indices of spaces
sp = np.where(characters == ' ')
# Result: (array([ 0, 6, 7, 13, 14], dtype=int64),)
# A little trick: let's double each index, and then take pairs
sp2 = np.repeat(sp, 2)
# Result: array([ 0, 0, 6, 6, 7, 7, 13, 13, 14, 14], dtype=int64)
# Get the pairs as a 2D matrix, skip the first and the last
w_ranges = np.reshape(sp2[1:-1], (-1, 2))
# Result: array([[ 0,  6],
#                [ 6,  7],
#                [ 7, 13],
#                [13, 14]], dtype=int64)
# Remove the indexing to the spaces themselves
w_ranges = w_ranges[np.where(w_ranges[:, 1] - w_ranges[:, 0] > 2)]
# Result: array([[ 0,  6],
#                [ 7, 13]], dtype=int64)

# Voila! Words are in between spaces, given as pairs of indices
words = list(map(lambda r: characters[r[0] + 1:r[1]], w_ranges))
# Result: [array(['h', 'e', 'l', 'l', 'o'], dtype='<U1'), 
#          array(['w', 'o', 'r', 'l', 'd'], dtype='<U1')]
# Let's recode the characters as strings
swords = np.array(list(map(lambda w: ''.join(w).strip(), words)))
# Result: array(['hello', 'world'], dtype='<U5')

# Next, let's remove stop words
stop_words = np.array(list(set(open('../stop_words.txt').read().split(','))))
ns_words = swords[~np.isin(swords, stop_words)]

"""replace leet code"""
ns_words = list(map(lambda w: np.array(list(w)), ns_words))
leet_alpha = np.array(['4', '8', '<', '|)', '3', '|=', '[,', '#', '1', '_|', '|<', '|_', '44', '|\\|', '0', '|o', 'O_', '|2', '5', '7', '|_|', '\\/', 'VV', '%', '`/', '2'])
words_leet = list(map(lambda w : w.astype('|S1').view(np.uint8) - 97, ns_words))
words_leet = list(map(lambda w : leet_alpha[w], words_leet))
# Result: [array(['#', '3', '|_', '|_', '0'], dtype='<U3'),
#          array(['VV', '0', '|2', '|_', '|)'], dtype='<U3')]

"""recode as strings """
swords = np.array(list(map(lambda w: ''.join(w).strip(), words_leet)))

"""build 2-grams"""
a = swords.reshape((-1, 1)).astype(object)
# array([['#3|_|_0'], ['VV0|2|_|)']], dtype=object)
b = np.hstack((a[:-1] + " ", a[1:]))
# Result: array([['#3|_|_0 ', 'VV0|2|_|)']], dtype=object)
c = np.sum(b, axis = 1).astype('str')
# Result: array(['#3|_|_0 VV0|2|_|)'], dtype='<U17')

### Finally, count the word occurrences
uniq, counts = np.unique(c, axis=0, return_counts=True)
wf_sorted = sorted(zip(uniq, counts), key=lambda t: t[1], reverse=True)

for w, c in wf_sorted[:5]:
    print(w, '-', c)