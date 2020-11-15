#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <algorithm>
using namespace std;

string raw;                         // raw data read from input
list<string> words;                 // words scanned
unordered_map<string, int> freq;
vector<pair<string, int>> res;      // top-25

// read the file content to a string variable
void readFile(const string &path) {
    ifstream file(path);
    raw = string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
}

// filter chars and normalize
void filter() {
    for (char &c : raw) 
        c = isalpha(c) ? tolower(c) : ' ';
}

// scan words in string into a list
void scan() {
    string word;
    stringstream ss(raw);
    while (ss >> word) {
        if (word.size() < 2)    continue;
        words.push_back(word);
    }
}

// remove stop words in global variable words
void removeStop() {
    ifstream stopfile("../stop_words.txt");
    if (!stopfile.is_open())    return;

    string word;
    unordered_set<string> stops;
    while (getline(stopfile, word, ',')) 
        stops.insert(word);

    for (auto it = words.begin(); it != words.end();) {
        if (stops.count(*it))  words.erase(it++);
        else it++;
    }
}

// count frequencies of each word
// save results to global variable freq
void frequencies() { 
    for (auto &word : words) {
        freq[word]++;
    } 
}

// get the top-25 words
// save results to global variable res
void sort() {
    for (auto &kv : freq) 
        res.push_back(kv);

    sort(res.begin(), res.end(), [](pair<string, int> &a, pair<string, int> &b) {
        return a.second > b.second;});
}

int main() {
    readFile("../pride-and-prejudice.txt");
    filter();
    scan();
    removeStop();
    frequencies();
    sort();

    for (int i = 0; i < 25; i++) 
        cout << res[i].first << "  -  " << res[i].second << endl;
}