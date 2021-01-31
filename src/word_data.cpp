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

void word_data::set_placement(const int row, const int col, const direction dirn)
{
    placed_row = row;
    placed_col = col;
    placed_direction = dirn;
}

void word_data::print_placement()
{
    std::cerr << clean_word << "\t\t";
    std::cerr << "placed at: " << placed_row << ", " << placed_col;
    std::cerr << ", " << direction_names[static_cast<int>(placed_direction)] << std::endl;
}

std::ostream& operator<<(std::ostream& os, const word_data &rhs)
{
    os << rhs.clean_word << " (" << rhs.letter_count << ")";
    return os;
}
