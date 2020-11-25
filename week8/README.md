## Style

### Chapter 3: Arrays
- Main data type: array – a fixed-size collection of elements.  
- No explicit iteration; instead, an array is accessed by high-level, declarative operations.  
- Computation unfolds as search, selection, and transformation of fixedsize data. 



> Using Python+numpy, or any other **array programming language**, implement a program that takes as input an array of characters (the characters from Pride and Prejudice, for example), normalizes them, and replaces all characters with their [Leet](https://simple.wikipedia.org/wiki/Leet)  counterparts. Then it prints out the 5 most frequently occurring 2-grams. (2-grams are all 2 consecutive words in a sequence)

> Note that you should stick to the array programming style as much as possible. This means: avoid explicit iteration over the elements of the array. If you find yourself writing an iteration, think of how you could do it with one or more array operations or with map/reduce operations. (Sometimes, it's not possible; but most often it is)

> See starting code [here](https://github.com/crista/exercises-in-programming-style/tree/master/03-arrays).



## Run

```shell
cd week8
```

```shell
python week8.py ../pride-and-prejudice.txt 
```

Note that the result **without leet replacement** is:
```
mr darcy - 273
mrs bennet - 153
mr collins - 150
lady catherine - 116
mr bingley - 115
```