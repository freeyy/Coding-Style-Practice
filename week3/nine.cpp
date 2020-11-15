#include "week3.h"

// C++ is not a dynamic language
// types of all func pointers should be determined at first
typedef void _fprint(vector<pair<string, int>>);
typedef void _fsort(unordered_map<string,int>, _fprint);
typedef void _ffreq(list<string>, _fsort);
typedef void _fstop(list<string>, _ffreq);
typedef void _fscan(string, _fstop);
typedef void _ffilter(string, _fscan);

void printRes(vector<pair<string, int>> res) {
    for (int i = 0; i < 25; i++) {
        cout << res[i].first << "  -  " << res[i].second << endl;
    }
}

void sort(unordered_map<string, int> freqs, _fprint func) {
    vector<pair<string, int>> res(freqs.begin(), freqs.end());
    sort(res.begin(), res.end(), [](pair<string, int> &a, pair<string, int> &b) {
        return a.second > b.second;});
    func(res);
}

void frequencies(list<string> words, _fsort func) { 
    unordered_map<string, int> freqs;
    for (auto &word : words) {
        freqs[word]++;
    } 
    func(freqs, printRes);
}

void removeStop(list<string> words, _ffreq func) {
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
    func(words, sort);
}

void scan(string data, _fstop func) {
    string word;
    stringstream ss(data);

    list<string> words;
    while (ss >> word) {
        if (word.size() < 2)    continue;
        words.push_back(word);
    }

    func(words, frequencies);
}

void filter(string data, _fscan func) {
    for (char &c : data) 
        c = isalnum(c) ? tolower(c) : ' ';
    func(data, removeStop);
}

void readFile(const string path,  _ffilter func) {
    ifstream file(path);
    string data = string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
    func(data, scan);
}

int main() {
    const string path = "../pride-and-prejudice.txt";
    readFile(path, filter);
}