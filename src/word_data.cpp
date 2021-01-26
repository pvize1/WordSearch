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

void word_data::set_placement(int row, int col, direction dirn)
{
    placed_row = row;
    placed_col = col;
    placed_direction = dirn;
}

std::vector<int> word_data::get_placement()
{
   return {placed_row, placed_col, static_cast<int>(placed_direction)};
}

std::ostream& operator<<(std::ostream& os, const word_data &rhs)
{
    os << rhs.clean_word << " (" << rhs.letter_count << ")";
    return os;
}
