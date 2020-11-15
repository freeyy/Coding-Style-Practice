#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <algorithm>
using namespace std;


int main() {
    string word;
    unordered_set<string> stops;
    unordered_map<string, int> freqs;
    // read stop words
    ifstream stopfile("../stop_words.txt");
    while (getline(stopfile, word, ','))    stops.insert(word);
    // read input and normalize
    ifstream file("../pride-and-prejudice.txt");
    string data = string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
    for (char &c : data)    c = tolower(c);
    // use regular expresion to fetch words
    regex rg("[a-z]{2,}");
    for (auto it = sregex_iterator(data.begin(), data.end(), rg); it != sregex_iterator(); it++) {
        string word = (*it).str();
        if (!stops.count(word))  freqs[word]++;
    }
    // sort by freq
    vector<pair<string, int>> res(freqs.begin(), freqs.end());
    sort(res.begin(), res.end(), [](pair<string, int> &a, pair<string, int> &b) {
        return a.second > b.second;});
    // print result
    for (int i = 0; i < 25; i++)  cout << res[i].first << "  -  " << res[i].second << endl; 
}