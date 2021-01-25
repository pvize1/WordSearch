#include "word_data.h"

word_data::word_data()
    : word_data("BLANK")
{
    //ctor
}

word_data::word_data(std::string new_word)
    : clean_word {new_word}
{
    letter_count = new_word.size();
}

word_data::~word_data()
{
    //dtor
}

void word_data::set_word(std::string new_word)
{
    clean_word = new_word;
}

std::string word_data::get_word()
{
    return clean_word;
}

int word_data::get_letter_count()
{
    return letter_count;
}

void word_data::set_placement(int row, int col, int direction)
{
    placed_row = row;
    placed_col = col;
    placed_direction = direction;
}

std::vector<int> word_data::get_placement()
{
   return {placed_row, placed_col, placed_direction};
}

