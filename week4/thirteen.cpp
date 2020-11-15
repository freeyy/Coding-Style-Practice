#include "week4.h"
/* Style: Closed Maps */

void extractWords(unordered_map<string, any> &obj, const string &path) {
    ifstream file(path);
    string data = string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
    for (char &c : data)    c = isalnum(c) ? tolower(c) : ' ';

    string word;
    list<string> words;
    stringstream ss(data);
    while (ss >> word) {
        if (word.size() > 1)
            words.push_back(word);
    }
    obj["data"] = words;
}

void loadStopWords(unordered_map<string, any> &obj) {
    string word;
    ifstream stopfile("../stop_words.txt");

    unordered_set<string> words;    
    while (getline(stopfile, word, ',')) 
        words.insert(word);
    
    obj["stop_word"] = words;
}

void incrementCount(unordered_map<string, any> &obj, string &word) {
    auto &freqs =  any_cast<unordered_map<string, int>&>(obj["freqs"]);
    freqs[word]++;
}

vector<pair<string, int>> sorted(unordered_map<string, any> &obj) {
    auto &freqs = any_cast<unordered_map<string, int>&>(obj["freqs"]);
    vector<pair<string, int>> res(freqs.begin(), freqs.end());
    sort(res.begin(), res.end(), [](pair<string, int> &a, pair<string, int> &b) {
        return a.second > b.second;});
    return res;
}

unordered_map<string, any> data_storage_obj {
    {"data", list<string> ()},
    {"init", function<void(string&)> {[](string &path){
        extractWords(data_storage_obj, path);}}},
    {"words", function<list<string>()> {[](){
        return any_cast<list<string>>(data_storage_obj["data"]);}}}
};

unordered_map<string, any> stop_words_obj {
    {"stop_word", unordered_set<string> ()},
    {"init", function<void()> {[](){
        loadStopWords(stop_words_obj);}}},
    {"is_stop_word", function<bool(string&)> {[](string &word){
        return any_cast<unordered_set<string>>(stop_words_obj["stop_word"]).count(word);}}},
};

unordered_map<string, any> word_freqs_obj {
    {"freqs", unordered_map<string, int> ()},
    {"increment_count", function<void(string&)> {[](string &word){
        incrementCount(word_freqs_obj, word);}}},
    {"sorted", function<vector<pair<string, int>>()> {[]() {
        return sorted(word_freqs_obj);}}}
};

int main(int argc, char *argv[]) {
    string path = argv[1];
    any_cast<function<void(string&)>>(data_storage_obj["init"])(path);
    any_cast<function<void()>>(stop_words_obj["init"])(); 

    for (string &word : any_cast<function<list<string>()>>(data_storage_obj["words"])()) {           
        if (!any_cast<function<bool(string&)>>(stop_words_obj["is_stop_word"])(word)) {
            any_cast<function<void(string&)>>(word_freqs_obj["increment_count"])(word);
        }
    }

    /** Exrcise 13.2: Add method "top25" **/
    word_freqs_obj["top25"] = function<void()> {[]() {
        auto freqs = any_cast<function<vector<pair<string, int>>()>>(word_freqs_obj["sorted"])();
        for (int i = 0; i < 25; i++) {
            cout << freqs[i].first << "  -  " << freqs[i].second << endl;
        }
    }};
    /******************************/
    
    // RUN
    any_cast<function<void()>>(word_freqs_obj["top25"])();
}