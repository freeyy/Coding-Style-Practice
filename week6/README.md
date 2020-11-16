## Style

### Chapter 29: Actors
- The larger problem is decomposed into things that make sense for the  problem domain.  
- Each thing has a queue meant for other things to place messages in it. 
- Each thing is a capsule of data that exposes only its ability to receive messages via the queue.  
- Each thing has its own thread of execution independent of the others.  

### Chapter 30: Dataspaces
- Existence of one or more units that execute concurrently.  
- Existence of one or more data spaces where concurrent units store and retrieve data.  
- No direct data exchanges between the concurrent units, other than via the data spaces. 


### Chapter 32: Map Reduce (double)
- Input data is divided in blocks.  
- A map function applies a given worker function to each block of data,  potentially in parallel.  
- The results of the many worker functions are reshuffled.  
- The reshuffled blocks of data are given as input to a second map function  that takes a reducible function as input.  
- Optional step: a reduce function takes the results of the many worker  functions and recombines them into a coherent 

> There are three exercises:
> 
> 1.  From Chapter 29, exercise 29.1
> 2.  From Chapter 30, exercise 30.1
> 3.  From Chapter 32, exercise 32.1, but with 32.3 grouping.


## Run

### Ex 29.1
```shell
go build -o out TwentieNine.go && ./out ../pride-and-prejudice.txt && rm out 
```

### Ex 30.1
```shell
go build -o out Thirty.go && ./out ../pride-and-prejudice.txt && rm out 
```

### Ex 32.1 (+32.3)
```shell
go build -o out ThirtyTwo.go && ./out ../pride-and-prejudice.txt && rm out 
```