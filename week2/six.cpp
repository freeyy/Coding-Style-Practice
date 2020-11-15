#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
using namespace std;

// read the file
// return entire content as string data
string readFile(const string path) {
    ifstream file(path);
    return string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
}

// filter chars and normalize in data
string filter(string data) {
    for (char &c : data) 
        c = isalnum(c) ? tolower(c) : ' ';
    return data;
}

// scan words in string
// return list of words
list<string> scan(string data) {
    string word;
    stringstream ss(data);

    list<string> words;
    while (ss >> word) {
        if (word.size() < 2)    continue;
        words.push_back(word);
    }

    return words;
}

// remove stop words from the list
list<string> removeStop(list<string> words) {
    ifstream stopfile("../stop_words.txt");
    if (!stopfile.is_open())    return{};

    string word;
    unordered_set<string> stops;
    while (getline(stopfile, word, ',')) 
        stops.insert(word);

    for (auto it = words.begin(); it != words.end();) {
        if (stops.count(*it))  words.erase(it++);
        else it++;
    }
    return words;
}

// count freq
unordered_map<string, int> frequencies(list<string> words) { 
    unordered_map<string, int> freqs;
    for (auto &word : words) {
        freqs[word]++;
    } 
    return freqs;
}

// sort by freq
vector<pair<string, int>> sort(unordered_map<string, int> freqs) {
    vector<pair<string, int>> res(freqs.begin(), freqs.end());
    sort(res.begin(), res.end(), [](pair<string, int> &a, pair<string, int> &b) {
        return a.second > b.second;});
    return res;
}

void printRes(vector<pair<string, int>> res) {
    for (int i = 0; i < 25; i++) {
        cout << res[i].first << "  -  " << res[i].second << endl;
    }
}

int main() {
    const string path = "../pride-and-prejudice.txt";
    printRes(sort(frequencies(removeStop(scan(filter(readFile(path)))))));
}