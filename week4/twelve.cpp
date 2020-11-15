#include "week4.h"
/* Style: letterbox */

// Models the contents of the file
class DataStorageManager {
private:
    list<string> words;

    void init(const string &path) {
        ifstream file(path);
        string data = string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
        for (char &c : data)    c = isalnum(c) ? tolower(c) : ' ';

        string word;
        stringstream ss(data);
        while (ss >> word) {
            if (word.size() > 1)
                words.push_back(word);
        }
    }

public:
    DataStorageManager() = default;

    any dispatch(const vector<string> &message) {
        if (message[0] == "init")   init(message[1]);
        else if (message[0] == "words") return words;
        else    throw invalid_argument("Unkown message");
        return 0;
    }
};

// Models the stop word filter
class StopWordManager {
private:
    unordered_set<string> stops;

    void init() {
        ifstream stopfile("../stop_words.txt");

        string word;
        while (getline(stopfile, word, ',')) 
            stops.insert(word);
    }

    bool isStop(const string &word) {
        return stops.count(word);
    }

public:
    StopWordManager() = default;

    any dispatch(const vector<string> &message) {
        if (message[0] == "init")   init();
        else if (message[0] == "is_stop_word")  return isStop(message[1]);
        else    throw invalid_argument("Unkown message");
        return 0;
    }
};

// Keeps the word frequency data 
class WordFrequencyManager {
private:
    unordered_map<string, int> freqs;

    void incrementCount(const string &word) {
        freqs[word]++;
    }

    vector<pair<string, int>> sorted() {
        vector<pair<string, int>> res(freqs.begin(), freqs.end());
        sort(res.begin(), res.end(), [](pair<string, int> &a, pair<string, int> &b) {
            return a.second > b.second;});
        return res;
    }
     
public:
    WordFrequencyManager() = default;

    any dispatch(const vector<string> &message) {
        if (message[0] == "increment_count")    incrementCount(message[1]);
        else if (message[0] == "sorted")    return sorted();
        else    throw invalid_argument("Unkown message");
        return 0;
    }
};

class WordFrequencyController {
private:
    DataStorageManager storageManager;
    StopWordManager stopWordManager;
    WordFrequencyManager wordFrequencyManager;

    void init(const string &path) { 
        storageManager.dispatch({"init", path});
        stopWordManager.dispatch({"init"});
    }

    void run() {
        for (string &word : any_cast<list<string>>(storageManager.dispatch({"words"}))) {
            if (!any_cast<bool>(stopWordManager.dispatch({"is_stop_word", word}))) {
                wordFrequencyManager.dispatch({"increment_count", word});
            }
        }
        
        auto freqs = any_cast<vector<pair<string, int>>>(wordFrequencyManager.dispatch({"sorted"}));
        for (int i = 0; i < 25; i++) {
            cout << freqs[i].first << "  -  " << freqs[i].second << endl;
        }
    }

public:
    any dispatch(const vector<string> &message) {
        if (message[0] == "init")   init(message[1]);
        else if (message[0] == "run")   run();
        else throw invalid_argument("Unknow message");
        return 0;
    }

};


int main(int argc, char *argv[]) {
    WordFrequencyController wfc;
    wfc.dispatch({"init", argv[1]});
    wfc.dispatch({"run"});
}