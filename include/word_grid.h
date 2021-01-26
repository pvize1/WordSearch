#ifndef WORD_GRID_H
#define WORD_GRID_H

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<random>
#include<ctime>

#include "word_list.h"
#include "word_data.h"


class Word_Grid
{
    public:
        Word_Grid();
        Word_Grid(int set_level);
        virtual ~Word_Grid();
        int read_and_clean_word_list(std::string filename);
        bool create_grid();
        bool place_all_words();
        void fill_blanks(std::vector<int> &fill_chars);
        void print_grid(bool as_char);

    private:
        Word_List words {};
        std::vector<word_data> clean_list {};
        int word_count {0};
        std::vector <std::vector<int>> word_grid {};
        std::vector <std::vector<int>> available_slots {};
        int longest_word_length {0};
        int grid_size {0};

        int place_word(word_data curr_word, bool use_slots);
        void print_placement(std::vector<int> &curr_word, int col, int row, int direction);
        std::vector <std::vector<int>> get_slots(std::vector<int> &curr_word);

        // initialise random device and seed
        std::random_device rd {};
        std::mt19937 _rnd { rd() };
        //std::default_random_engine _rnd {};

        //TODO enum struct level {EASY, MEDIUM, HARD} grid_level;
        int level {};
};


#endif // WORD_GRID_H
