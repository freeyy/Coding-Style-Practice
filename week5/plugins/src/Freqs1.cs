using System;
using System.Linq;
using System.Collections.Generic;

namespace Plugin
{
    public class Freqs
    {
        public List<KeyValuePair<string, int>> top25(string[] words)
        {
            var freqs = words.GroupBy(x => x).ToDictionary(x => x.Key, x => x.Count());
            return freqs.OrderByDescending(x => x.Value).Take(25).ToList();
        }
    }
}