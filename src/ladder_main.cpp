#include "ladder.h"

int main() {
    try {
        set<string> word_list;
        load_words(word_list, "words.txt");
        cout << "Loaded " << word_list.size() << " words from dictionary." << endl;
        
        char option;
        cout << "Run verification tests? (y/n): ";
        cin >> option;
        
        if (option == 'y' || option == 'Y') {
            verify_word_ladder();
            return 0;
        }
        
        string start_word, end_word;
        cout << "Enter start word: ";
        cin >> start_word;
        cout << "Enter end word: ";
        cin >> end_word;
        
        vector<string> ladder = generate_word_ladder(start_word, end_word, word_list);
        print_word_ladder(ladder);
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}