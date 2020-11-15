## Style



## Run

### Ex 17

The program will first run `WordCount`.
Then it will ask for input of one `classname` and show the corresponding info.

**Compile and run**:
``` shell
mcs seventeen.cs 
mono ./seventeen.exe ../pride-and-prejudice.txt
```

### Ex 20.1

DLL files have been pre-compliled in `./plugins`.
You can see the source file in `./plugins/src`.

In `config.xml`, you can switch plugins by modifying:
- `plugins/Words1.dll` and `plugins/Words2.dll`
- `plugins/Freqs1.dll` and `plugins/Freqs2.dll`


**Compile and run**
``` shell
mcs twentie.cs
mono ./twentie.exe ../pride-and-prejudice.txt
```

**NOTE**: Here the `twentie.exe` is equivalent to the required third DLL, i.e. `framework.dll`.