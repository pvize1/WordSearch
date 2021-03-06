#include <string>
#include "word_grid.h"


int main()
{
    const std::string FILE_NAME = "C:\\Users\\peter\\c++\\WordSearch\\words_days_months.txt";
    Word_Grid grid(1);
    int result {0};
    int num_tries {0};

    result = grid.read_and_clean_word_list(FILE_NAME);

    if ( result == 0 )
    {
        while ( num_tries < 10 )
        {
            num_tries++;
            if ( !grid.create_grid() )
            {
                std::cerr << "Could not create grid" << std::endl;
                return 2;
            }


            if ( grid.place_all_words() )
            {
                //TODO using set of all letters in clean_list
                std::vector<int> fill_letters {65,66,67,68,69,70,71,72};
                grid.fill_blanks(fill_letters);
                grid.print_grid(true);
                grid.print_all_placements();

                return 0;
            }
            else
                std::cout << std::endl << "All words not placed, trying again... (" << num_tries << ")" << std::endl;
        }
    }
    return result;
}
