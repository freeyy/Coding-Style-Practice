using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;

namespace Plugin
{
    public class Words{
        public string[] extractWords(string path, string stoppath)
        {            
            string data = File.ReadAllText(path);
            data = new string(data.Select(c => (Char.IsLetter(c) ? Char.ToLower(c) : ' ')).ToArray());
            var lwords = data.Split(' ');

            HashSet<string> stops = new HashSet<string>();
            foreach (string word in File.ReadAllText(stoppath).Split(','))
                stops.Add(word);
            
            return lwords.Where(x => (x.Length > 1 && !stops.Contains(x))).ToArray();
        }
    }
}