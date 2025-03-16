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
        int diff = 0;
        for (int i = 0; i < len1; ++i) {
            if (str1[i] != str2[i]) {
                diff++;
                if (diff > d) {
                    return false;
                }
            }
        }
        return true;
    }
    
    if (len1 > len2) {
        return edit_distance_within(str2, str1, d);
    }
    
    for (int i = 0, j = 0; i <= len1; ++i, ++j) {
        if (i < len1 && str1[i] == str2[j]) {
            continue;
        }
        
        --i;
        
        if (d <= 0) {
            return false;
        }
        d--;
    }
    
    return true;
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
    
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin});
    
    set<string> visited;
    visited.insert(begin);
    
    if (word_list.find(end) == word_list.end()) {
        error(begin_word, end_word, "End word is not in the dictionary");
        return {};
    }
    
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = ladder.back();
        
        if (last_word == end) {
            return ladder;
        }
        
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
        cout << "No ladder found." << endl;
        return;
    }
    
    cout << "Found ladder of length " << ladder.size() << ":" << endl;
    for (size_t i = 0; i < ladder.size(); ++i) {
        cout << ladder[i];
        if (i < ladder.size() - 1) {
            cout << " → ";
        }
    }
    cout << endl;
}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    
    cout << "Testing generate_word_ladder:" << endl;
    
    auto test = [&](const string& start, const string& end, int expected_size) {
        vector<string> ladder = generate_word_ladder(start, end, word_list);
        bool passed = ladder.size() == expected_size;
        cout << start << " to " << end << ": " << (passed ? "passed" : "failed") 
             << " (expected " << expected_size << ", got " << ladder.size() << ")" << endl;
        
        if (!passed) {
            print_word_ladder(ladder);
        }
    };
    
    test("cat", "dog", 4);
    test("marty", "curls", 6);
    test("code", "data", 6);
    test("work", "play", 6);
    test("sleep", "awake", 8);
    test("car", "cheat", 4);
}