# Style
## Chapter 35: Dense, Shallow, under  Control 
- The neural function consists of one single layer that connects all inputs  to all outputs. 
- The neural function is hardcoded by human programmers by setting the  values of the weights explicitly. 

## Chapter 36: Dense, Shallow, out of  Control 
- The neural function consists of one single layer that connects all inputs to all outputs. 
- The neural function is learned via inferences on training data. 

## Chapter 40: Recurrent
- The input is a sequence of items, and the output depends on certain  patterns in that sequence.  
- The length of the output sequence is exactly the same as the length of the input sequence.  
- The input is reshaped as a sequence of frames of size N, each one capturing a portion of the input sequence, large enough to be able to capture the pattern. 
- The frames are created by sliding through the input sequence, with a step of 1. 
- The neural function is defined as a single unit that is instantiated N  times and applied to all items in a frame, at the same time. Those instances are connected in a chain where the output of one feeds as  input into the next. Each unit has, therefore, two sets of weights: one  for the items from the input sequence, the other for the output of the  previous unit in the chain. 


> **Preliminaries**: install [Anaconda](https://www.anaconda.com/). Then make sure that the Keras environment is active in Anaconda (see [this post](https://towardsdatascience.com/https-medium-com-ekapope-v-install-tensorflow-and-keras-using-anaconda-navigator-without-command-line-b0bc41dbd038) \-\- note that it takes some time to install everything needed). For running Keras programs, call Python from the Anaconda command prompt.
>
>**Exercises:**
>- Exercise 35.3
>- Exercise 36.4
>- Exercise 40.2

## Run
- Please make sure `keras` is available in your python environment.
- We use `stop_words.txt` as an example input.


### Exercise 35.3
```shell
python ThirtyFive.py stop_words.txt
```

### Exercise 36.4
```shell
python ThirtySix.py stop_words.txt
```

### Exercise 40.2
```shell
python forty.py stop_words.txt 
```