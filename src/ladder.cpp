#include "ladder.h"

void error(string word1, string word2, string msg) {
    cerr << "Error for words '" << word1 << "' and '" << word2 << "': " << msg << endl;
}

bool edit_distance_one_insertion(const string& longer, const string& shorter) {
    for (size_t i = 0; i <= longer.length(); i++) {
        string test = longer.substr(0, i) + longer.substr(i+1);
        if (test == shorter) {
            return true;
        }
    }
    return false;
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
    
    if (len1 - len2 == 1) {
        return edit_distance_one_insertion(str1, str2);
    } else if (len2 - len1 == 1) {
        return edit_distance_one_insertion(str2, str1);
    }
    
    return false;
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
        error(begin_word, end_word, "End word must be in the dictionary");
        return {};
    }
    
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin});
    
    set<string> visited;
    visited.insert(begin);
    
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
    
    // No ladder found
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
    
    #define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}
    
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}