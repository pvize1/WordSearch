#ifndef WORD_DATA_H
#define WORD_DATA_H

#include <vector>
#include <string>


class word_data
{
    public:
        word_data();
        word_data(std::string new_word);
        virtual ~word_data();
        void set_word(std::string new_word);
        std::string get_word();
        int get_letter_count();
        void set_placement(int row, int col, int direction);
        std::vector<int> get_placement();

    protected:

    private:
        std::string clean_word {};
        int letter_count {0};
        int placed_row {0};
        int placed_col {0};
        int placed_direction {0};
};

#endif // WORD_DATA_H
