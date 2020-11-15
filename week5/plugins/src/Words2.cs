using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text.RegularExpressions;

namespace Plugin
{
    public class Words{
        public string[] extractWords(string path, string stoppath)
        {            
            var words = new List<string>();
            var stops = new HashSet<string>(File.ReadAllText(stoppath).Split(','));

            Regex rx = new Regex("[a-z]{2,}");
            var matches = rx.Matches(File.ReadAllText(path).ToLower());
            foreach (Match match in matches) {
                string word = match.Value;
                if (word.Length > 1 && !stops.Contains(word))
                    words.Add(match.Value);
            }

            return words.ToArray();
        }
    }
}