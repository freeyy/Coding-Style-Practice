#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
using namespace std;

int main() {
    unordered_set<string> stops;
    unordered_map<string, int> freqs;

    // read stop words
    string word;
    ifstream stopfile("../stop_words.txt");
    if (!stopfile.is_open())    return -1;
    while (getline(stopfile, word, ',')) {
        stops.insert(word);
    }

    // read input file
    ifstream infile("../pride-and-prejudice.txt");
    if (!infile.is_open())    return -1;

    // count word freqs
    string line;
    while (getline(infile, line)) { // for each line
        // to_lowercase and replace non-letter to space
        transform(line.begin(), line.end(), line.begin(),[](char c) {
            return isalpha(c) ? tolower(c) : ' ';
        });
        string word;
        stringstream ss(line);
        while (ss >> word) {    // for each word
            if (word.size() < 2 || stops.count(word))
                continue;
            freqs[word]++; 
        }
    }

    // use a min-heap to get the top-25
    priority_queue<pair<int, string>, vector<pair<int, string>>, 
        greater<pair<int,string>>> q;
    for (auto &kv : freqs) {
        q.push({kv.second, kv.first});
        if (q.size() > 25)  q.pop();
    }

    // output result
    vector<pair<int, string>> res; 
    while (!q.empty()) {
        res.push_back(q.top());
        q.pop();
    }
    for (int i = res.size() - 1; i >= 0; i--)
        cout << res[i].second << "  -  " << res[i].first << endl;
    
    return 0;
}