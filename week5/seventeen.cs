using System;
using System.IO;
using System.Text.RegularExpressions;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace seventeen
{
    abstract class TFExercise 
    { 
        protected string info()
        {
            return this.GetType().Name;
        }
    }

    class DataStorageManager : TFExercise
    {
        string[] words;

        public DataStorageManager(string path) 
        {
            string data = File.ReadAllText(path);
            data = new string(data.Select(c => (Char.IsLetter(c) ? Char.ToLower(c) : ' ')).ToArray());
        
            var lwords = data.Split(" ", StringSplitOptions.RemoveEmptyEntries);
            words = lwords.Where(x => (x.Length > 1)).ToArray();
        }

        public string[] GetWords() 
        {
            return this.words; 
        }
    }

    class StopWordManager : TFExercise 
    {
        HashSet<string> stops = new HashSet<string>();

        public StopWordManager() 
        {
            string data = File.ReadAllText("../stop_words.txt");
            foreach (string word in data.Split(","))
                stops.Add(word);
        }

        public bool IsStopWord(string word)
        {
            return stops.Contains(word);
        }
    } 

    class WordFreqManager : TFExercise
    {
        Dictionary<string, int> freqs = new Dictionary<string, int>();

        public void IncrementCount(string word)
        {
            if (freqs.ContainsKey(word))    freqs[word]++;
            else    freqs.Add(word, 1);
        }

        public List<KeyValuePair<string, int>> Sorted() 
        {
            var list = freqs.ToList();
            list.Sort((x, y) => y.Value.CompareTo(x.Value));
            return list;
        }

    }

    class WordFreqController : TFExercise
    {
        DataStorageManager storageManager;
        StopWordManager stopWordManager;
        WordFreqManager wordFreqManager;

        public WordFreqController(string path) 
        {
            storageManager = new DataStorageManager(path);
            stopWordManager = new StopWordManager();
            wordFreqManager = new WordFreqManager();
        }

        public void run() 
        {
            // invoke methods using reflection
            MethodInfo fGetWords = storageManager.GetType().GetMethod("GetWords");
            MethodInfo fIsStopWord = stopWordManager.GetType().GetMethod("IsStopWord"); 
            MethodInfo fIncrementCount = wordFreqManager.GetType().GetMethod("IncrementCount"); 
            MethodInfo fSorted = wordFreqManager.GetType().GetMethod("Sorted");


            foreach (string word in (string[]) fGetWords.Invoke(storageManager, new object[]{}))
                if (!(bool) fIsStopWord.Invoke(stopWordManager, new object[]{word})) 
                    fIncrementCount.Invoke(wordFreqManager, new object[]{word});
            
            foreach (var kv in ((List<KeyValuePair<string, int>>)fSorted.Invoke(wordFreqManager, new object[]{})).GetRange(0, 25)) 
                Console.WriteLine(kv.Key + "  -  " + kv.Value);

        }
    }

    class Program
    {
        static void printClass(string classname)
        {
            classname = "seventeen." + classname;
            var classtype = Type.GetType(classname);
            if (classtype == null) {
                Console.WriteLine("Classname not found, program exits.");
                return;
            }

            var binds = BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic;
            // fields
            Console.WriteLine("\n[Fields]");
            var fields = classtype.GetFields(binds);
            foreach (var field in fields) 
                Console.WriteLine(field.ToString());
            // methods
            Console.WriteLine("\n[Methods]");
            var methods = classtype.GetMethods(binds);
            foreach (var method in methods)
                Console.WriteLine(method.ToString());
            // interface
            Console.WriteLine("\n[Interfaces]");
            var itfs = classtype.GetInterfaces();
            foreach (var itf in itfs)
                Console.WriteLine(itf.ToString());
            // superclass
            Console.WriteLine("\n[Superclass]");
            Console.WriteLine(classtype.BaseType);
        }

        static void Main(string[] args)
        {
            // word-count
            var wfc = new WordFreqController(args[0]);
            wfc.run();

            // meta-info
            Console.WriteLine("\nInput one of the following classname to see its info:");
            Console.WriteLine("- DataStorageManager");
            Console.WriteLine("- StopWordManager");
            Console.WriteLine("- WordFreqManager");
            Console.WriteLine("- WordFreqController");
            string classname = Console.ReadLine();
            printClass(classname);
        }
    }
}
