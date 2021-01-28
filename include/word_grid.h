#ifndef WORD_GRID_H
#define WORD_GRID_H

#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<random>
#include<ctime>
#include<memory>

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
        void print_all_placements();

    private:
        std::vector<std::shared_ptr<word_data>> clean_list;
        int word_count {0};
        std::vector <std::vector<int>> word_grid {};
        std::vector <std::vector<int>> available_slots {};
        int grid_size {0};

        int place_word(std::shared_ptr<word_data> curr_word_data, bool use_slots);
        std::vector <std::vector<int>> get_slots(const std::string &curr_word);

        // initialise random device and seed
        std::random_device rd {};
        std::mt19937 _rnd { rd() };
        //std::default_random_engine _rnd {};

        enum struct level {EASY, MEDIUM, HARD} grid_level;
        int level {};
};


#endif // WORD_GRID_H
