## Style

> There are two exercises:
> 
> 1.  Do a variation of 17.1 as explained here.  
>     Go back to Chapter 11, Things, which is a vanilla OOP design. Redo it in Java or C# (only). Then:
>     1.  In the run method of WordFrequencyController (or equivalent in your code), invoke the methods of the DataStorageManager, StopWordManager, and WordFrequencyCounter objects using reflection.
>     2.  In your main function (or main block), prompt the user for a class name of one of your application classes (e.g. DataStorageManager) and call a function that prints out all of that class's fields (their names and types), all of its method names, and all of its superclasses and implemented interfaces -- this should be done using reflection, and not hardcoded with conditionals (harcoding with conditionals would be you doing _if the user chose DataStorageManager let me printout the fields I know it has, etc._). The printout function should take a class name as string and should work in a generic manner for any class that exists in your application.  
>           
>         
> 2.  Do exercise 20.1 as specified in the book. If you are doing this in Java or C#, you should produce **three** jar (dll in C#) files: the framework package (framework.jar or Framework.dll), a package with one implementation of the words and frequencies functions (app1.jar or App1.dll), and another package with another implementation of the words and frequencies functions (app2.jar or App2.dll). One should be able to run the framework plus one of the apps from anywhere, as long as they have those packages and the configuration file.
> 
> Place both programs under a folder called Week5 in your development environment. Call them Seventeen.xxx  and Twenty.xxx, where xxx is the extension of the language you decide to use. If you have instructions for the Reader, write them in a file called README in the same folder.


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