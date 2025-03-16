#ifndef LADDER_H
#define LADDER_H

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <fstream>
#include <algorithm>

using namespace std;

#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}

void error(string word1, string word2, string msg);
bool edit_distance_within(const string& str1, const string& str2, int d);
bool is_adjacent(const string& word1, const string& word2);
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list);
void load_words(set<string>& word_list, const string& file_name);
void print_word_ladder(const vector<string>& ladder);
void verify_word_ladder();

#endif // LADDER_H
