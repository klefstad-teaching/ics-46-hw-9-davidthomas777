#include "ladder.h"

void error(string word1, string word2, string msg) {
    cerr << "Error for words '" << word1 << "' and '" << word2 << "': " << msg << endl;
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    int len1 = str1.length();
    int len2 = str2.length();
    
    if (abs(len1 - len2) > d) {
        return false;
    }
    
    if (len1 == len2) {
        int diffs = 0;
        for (int i = 0; i < len1; i++) {
            if (str1[i] != str2[i]) {
                diffs++;
                if (diffs > d) {
                    return false;
                }
            }
        }
        return true;
    }
    
    if (len1 > len2) {
        return edit_distance_within(str2, str1, d);
    }
    
    int i = 0, j = 0;
    bool skip_done = false;
    
    while (i < len1 && j < len2) {
        if (str1[i] != str2[j]) {
            if (skip_done) {
                return false;
            }
            skip_done = true;
            j++;
        } else {
            i++;
            j++;
        }
    }
    
    return (j == len2) || (j == len2 - 1);
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    string begin = begin_word;
    string end = end_word;
    transform(begin.begin(), begin.end(), begin.begin(), ::tolower);
    transform(end.begin(), end.end(), end.begin(), ::tolower);
    
    if (begin == end) {
        error(begin_word, end_word, "Start and end words are the same");
        return {};
    }
    
    if (word_list.find(end) == word_list.end()) {
        return {};
    }
    
    if (is_adjacent(begin, end)) {
        return {begin, end};
    }
    
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin});
    
    set<string> visited;
    visited.insert(begin);
    
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();
        for (const string& word : word_list) {
            if (visited.find(word) != visited.end()) {
                continue;
            }
            if (is_adjacent(last_word, word)) {
                visited.insert(word);
                vector<string> new_ladder = ladder;
                new_ladder.push_back(word);
                if (word == end) {
                    return new_ladder;
                }
                ladder_queue.push(new_ladder);
            }
        }
    }
    return {};
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file) {
        throw runtime_error("Cannot open dictionary file: " + file_name);
    }
    string word;
    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        word_list.insert(word);
    }
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); ++i) {
        cout << ladder[i];
        if (i < ladder.size() - 1) {
            cout << " ";
        }
    }
    cout << endl;
}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    cout << "Testing word ladder generation..." << endl;
    vector<pair<pair<string, string>, int>> test_cases = {
        {{"cat", "dog"}, 4},
        {{"marty", "curls"}, 6},
        {{"code", "data"}, 6},
        {{"work", "play"}, 6},
        {{"sleep", "awake"}, 8},
        {{"car", "cheat"}, 4}
    };
    
    for (const auto& test : test_cases) {
        string start = test.first.first;
        string end = test.first.second;
        int expected_size = test.second;
        vector<string> ladder = generate_word_ladder(start, end, word_list);
        cout << "From " << start << " to " << end << ": ";
        if (ladder.size() == expected_size) {
            cout << "PASSED (" << ladder.size() << " words)" << endl;
        } else {
            cout << "FAILED (expected " << expected_size << ", got " << ladder.size() << ")" << endl;
        }
    }
}