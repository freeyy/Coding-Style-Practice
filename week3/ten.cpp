/* Style: The One*/
#include "week3.h"

class TheOne {
private:
    any tmp;
public:
    TheOne(const std::string path) : tmp(path) {}

    // Keep when binding: former func's return type equals
    // to next func's input type
    template <class R, class Arg>
    TheOne* bindFunc(R(*func)(Arg)) {
        tmp = func(any_cast<Arg>(tmp));
        return this;
    }

    // Final type of tmp is string
    void printme() {
        cout << any_cast<std::string>(tmp) << endl;
    }
};

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
vector<pair<string, int>> sortByFreq(unordered_map<string, int> freqs) {
    vector<pair<string, int>> res(freqs.begin(), freqs.end());
    sort(res.begin(), res.end(), [](pair<string, int> &a, pair<string, int> &b) {
        return a.second > b.second;});
    return res;
}

// return the result string
string top25(vector<pair<string, int>> freqs) {
    stringstream ss;
    for (int i = 0; i < 25; i++) 
        ss << freqs[i].first << "  -  " << freqs[i].second << endl;
    return ss.str();
}

int main() {
    const string path = "../pride-and-prejudice.txt";
    TheOne one(path);
    one.bindFunc(readFile)->bindFunc(filter)->
        bindFunc(scan)->bindFunc(removeStop)->
        bindFunc(frequencies)->bindFunc(sortByFreq)->
        bindFunc(top25)->printme();
}