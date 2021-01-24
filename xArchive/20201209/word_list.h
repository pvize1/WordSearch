#ifndef WORD_LIST_H
#define WORD_LIST_H

#include<fstream>
#include<iostream>
#include<string>
#include<vector>

class Word_List
{
private:
    int word_count {0};
    int longest_word_length {0};
    int longest_word_index {0};
    std::vector<std::string> raw_list {};
    std::vector<std::vector<int>> clean_list {};

public:
    Word_List();
    virtual ~Word_List();
    bool read_raw_list(std::string file_name);
    void create_clean_list();
    bool is_word_match(std::vector<int> &word_1, std::vector<int> &word_2);
    void print_word_list();
    void print_word(int word_index, bool as_char);
    void print_placement(int word_index, int col, int row, int direction);
    std::vector<int> get_longest_word();
    int get_longest_word_length();
    int get_word_count();
    int get_longest_word_index();
    std::vector<int> get_word(int list_index);
};

class Word_Data
{

};

#endif // WORD_LIST_H
