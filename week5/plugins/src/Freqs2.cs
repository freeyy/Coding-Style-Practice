using System;
using System.Linq;
using System.Collections.Generic;

namespace Plugin
{
    public class Freqs
    {
        public List<KeyValuePair<string, int>> top25(string[] words)
        {
            var freqs = new Dictionary<string, int>();
            foreach (string word in words)
            {
                if (freqs.ContainsKey(word))    freqs[word]++;
                else    freqs.Add(word, 1);
            }

            return freqs.OrderByDescending(x => x.Value).Take(25).ToList();
        }
    }
}