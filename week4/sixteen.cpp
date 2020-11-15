#include "week4.h"
/* Style: Hollywood */


// The event manager (subscribe / publish)
class EventManager {
private:
    unordered_map<string, vector<function<void(vector<string>&)>>> subscriptions;
public:
    EventManager() = default;

    void subscribe(const string &eventType, function<void(vector<string>&)> cb) {
        subscriptions[eventType].push_back(cb); 
    }
    
    void publish(vector<string> events) {
        const string &eventType = events[0];

        for (auto &cb : subscriptions[eventType]) {
            cb(events);
        }
    }
};

/****************************/
/* The application entities */
/****************************/

// Models the contents of the file
class DataStorage {
private:
    EventManager &em;
    vector<string> words;
public:
    DataStorage(EventManager &em) : em(em) {
        // load input words
        em.subscribe("load", [&](vector<string> &events) {
            const string &path = events[1];
            ifstream file(path);
            string data = string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
            for (char &c : data)    c = isalnum(c) ? tolower(c) : ' ';

            string word;
            stringstream ss(data);
            while (ss >> word) {
                if (word.size() > 1)    
                    words.push_back(word);
            }
        });

        // produce words
        em.subscribe("start", [&](vector<string> &events) {
            for (string &word : words) 
                em.publish({"word", word});
            em.publish({"eof"});
        });
    }
};


// Models the stop word filter
class StopWordFilter {
private:
    EventManager &em;
    unordered_set<string> stopWords;
public:
    StopWordFilter(EventManager &em) : em(em) {
        // load stop words
        em.subscribe("load", [&](vector<string> &events) {
            ifstream stopfile("../stop_words.txt");
            string word;
            while (getline(stopfile, word, ',')) 
                stopWords.insert(word);
        });

        // push forward non-stop word
        em.subscribe("word", [&](vector<string> &events) {
            const string &word = events[1];
            if (!stopWords.count(word)) {
                em.publish({"valid_word", word});
            }
        });
    }
};


// Keeps the word frequency data
class WordFrequencyCounter {
private:
    EventManager &em;
    unordered_map<string, int> freqs;
public:
    WordFrequencyCounter(EventManager &em) : em(em) {
        // count word freqs
        em.subscribe("valid_word", [&](vector<string> &events) {
            const string &word = events[1];
            freqs[word]++;
        });

        // print freqs
        em.subscribe("print", [&](vector<string> &events) {
            vector<pair<string, int>> res(freqs.begin(), freqs.end());
            sort(res.begin(), res.end(), [](pair<string, int> &a, pair<string, int> &b) {
                return a.second > b.second;});

            for (int i = 0; i < 25; i++) 
                cout << res[i].first << "  -  " << res[i].second << endl;
        });
    }
};


// Exercise 16.2
class WordsWithZ {
private:
    EventManager &em;
    unordered_set<string> zwords;
public:
    WordsWithZ(EventManager &em) : em(em) {
        // count z-word 
        em.subscribe("valid_word", [&](vector<string> &events) {
            const string &word = events[1];
            if (word.find('z') != word.npos)    zwords.insert(word);
        });

        // print freqs
        em.subscribe("print", [&](vector<string> &events) {
            cout << endl << "#words with z: " << zwords.size() << endl;
        });
    }
};


// Entrance
class WordFrequencyApplication {
private:    
    EventManager &em;
public:
    WordFrequencyApplication(EventManager &em) : em(em) {
        // run the application
        em.subscribe("run", [&](vector<string> &events) {
            const string &path = events[1];
            em.publish({"load", path});
            em.publish({"start"});
        });

        // finish the application
        em.subscribe("eof", [&](vector<string> &events) {
            em.publish({"print"});
        });
    }
};

// 


int main(int argc, char *argv[]) {
    EventManager em;

    DataStorage ds(em);
    StopWordFilter sf(em);
    WordFrequencyCounter wfc(em);
    WordFrequencyApplication wfa(em);
    
    // Exercise 16.2: words with z
    // NOTE: possible ambiguity in the probelm,
    // here I understand it as "words contains z".
    WordsWithZ wz(em);

    // run
    em.publish({"run", argv[1]});
}