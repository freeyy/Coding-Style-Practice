using System;
using System.Linq;
using System.IO;
using System.Reflection;
using System.Xml;
using System.Collections.Generic;


namespace twentie
{
    class Program
    {
        static string pluginWords;
        static string pluginFreqs;

        static void loadConfig(string path) 
        {
            XmlDocument doc = new XmlDocument();
            doc.Load(path);
            var words = (XmlElement)doc.GetElementsByTagName("words")[0];
            var freqs = (XmlElement)doc.GetElementsByTagName("freqs")[0];
            pluginWords = words.GetAttribute("include");
            pluginFreqs = freqs.GetAttribute("include");
        }

        static void Main(string[] args)
        {
            Object wordsObj, freqsObj;
            MethodInfo extractWords, top25;

            try
            {
                // load config
                loadConfig("config.xml");
                // load words.dll
                Assembly plgwords = Assembly.LoadFrom(pluginWords);
                Type wordsClass = plgwords.GetType("Plugin.Words");
                wordsObj = Activator.CreateInstance(wordsClass);
                extractWords = wordsClass.GetMethod("extractWords");
                // load freqs.dll
                Assembly plgfreqs = Assembly.LoadFrom(pluginFreqs);
                Type freqsClass = plgfreqs.GetType("Plugin.Freqs");
                freqsObj = Activator.CreateInstance(freqsClass);
                top25 = freqsClass.GetMethod("top25");
            }
            catch (System.Exception)
            {
                Console.WriteLine("Something went wrong. Please check config.xml and .dll files.");
                return;
            }


            // run
            var words = (string[])extractWords.Invoke(wordsObj, new string[]{args[0], "../stop_words.txt"});
            var freqs = (List<KeyValuePair<string, int>>)top25.Invoke(freqsObj, new object[] {words});

            foreach (var kv in freqs)
                Console.WriteLine(kv.Key + "  -  " + kv.Value);
        }
    }
}
