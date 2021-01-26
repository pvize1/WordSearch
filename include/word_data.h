#ifndef WORD_DATA_H
#define WORD_DATA_H

#include <vector>
#include <string>
#include <iostream>


class word_data
{
    friend std::ostream& operator<<(std::ostream &os, const word_data &rhs);

    public:
        word_data();
        word_data(std::string new_word);
        virtual ~word_data();
        void set_word(std::string new_word);
        std::string get_word();
        int get_letter_count();
        enum struct direction {ACROSS, DOWN, DIAG} word_direction;
        void set_placement(int row, int col, direction direction);
        std::vector<int> get_placement();


    protected:

    private:
        std::string clean_word {};
        int letter_count {0};
        int placed_row {0};
        int placed_col {0};
        direction placed_direction {};
};

#endif // WORD_DATA_H
