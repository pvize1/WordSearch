#ifndef WORD_GRID_H
#define WORD_GRID_H

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<random>
#include<ctime>

#include "word_list.h"


class Word_Grid
{
    private:
        std::string file_name {};
        Word_List words {};
        std::vector <std::vector<int>> word_grid {};
        std::vector <std::vector<int>> available_slots {};
        int grid_size {0};
        // initialise random device and seed
        std::random_device rd {};
        std::mt19937 _rnd { rd() };
        //std::default_random_engine _rnd {};
        enum struct direction {ACROSS, DOWN, DIAG} word_direction;
        int level {};
        //enum struct level {EASY, MEDIUM, HARD} grid_level; TODO
        std::vector<int> place_word(std::vector<int> curr_word, bool use_slots);
        void print_placement(std::vector<int> &curr_word, int col, int row, int direction);
        std::vector <std::vector<int>> get_slots(std::vector<int> &curr_word);

    public:
        Word_Grid();
        Word_Grid(int set_level);
        virtual ~Word_Grid();
        bool read_and_clean_word_list(std::string filename);
        bool create_grid();
        int get_grid_size();
        bool place_all_words();
        void fill_blanks(std::vector<int> &fill_chars);
        void print_grid(bool as_char);
};


#endif // WORD_GRID_H
