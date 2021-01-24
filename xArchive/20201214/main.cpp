//#include<iostream>
#include<string>
#include<vector>
#include<random>
#include "word_grid.h"
#include "word_list.h"


enum struct direction {ACROSS, DOWN, DIAG} word_direction;
//enum struct level {EASY, MEDIUM, HARD} grid_level; TODO

void print_placement(std::vector<int> &curr_word, int col, int row, int direction);
std::vector <std::vector<int>> get_slots(std::vector <std::vector<int>> &grid, int grid_size, std::vector<int> &curr_word);
bool can_place_word(std::vector <std::vector<int>> &grid, int col, int row, direction word_direction, std::vector<int> &curr_word);
void place_word(std::vector <std::vector<int>> &grid, int col, int row, direction word_direction, std::vector<int> &curr_word);
void fill_blanks(std::vector <std::vector<int>> &grid, int grid_size, std::vector<int> &fill_chars);
void print_grid(std::vector <std::vector<int>> &grid, int grid_size, bool as_char);

std::random_device rd{};
auto mtgen = std::mt19937
{
    rd()
};


int main()
{
    const std::string FILE_NAME = "C:\\Users\\peter\\c++\\WordSearch\\words_santa.txt";

    std::vector <std::vector<int>> word_grid {};
    std::vector <std::vector<int>> available_slots {};
    std::vector<int> curr_word {0};
    int curr_word_size {0};
    int grid_size {0};
    int row {0};
    int col {0};
    int slot_no {0};
    Word_List words {};

    //Process the raw list of words, clean (remove whitespace) and upper
    // TODO (peter#1#): Change to return error value ie 0=Success, 1=File Not Found, 2=No Valid Words
    if ( words.create_clean_list(FILE_NAME) )
        words.print_word_list();
    else
    {
        std::cerr << std::endl << "File not found (" << FILE_NAME << ")" << std::endl;
        return 1;
    }




    // ------------------------------------------------------------------
    // Set-up word grid based on longest word and pre-populate with zeros
    // ------------------------------------------------------------------
    auto filler = std::uniform_int_distribution<> { 1, 7 };
    grid_size = words.get_longest_word_length() + filler(mtgen);
    word_grid.resize(grid_size,std::vector<int>(grid_size, 0));
    std::cerr << "Grid Size=" << grid_size << std::endl;
    std::cerr << std::endl;
    std::cerr << "------------------------------------------------";
    std::cerr << std::endl << std::endl;

    // --------------------------------------------------------------------
    // Place first word randomly, in random direction
    // --------------------------------------------------------------------
    auto dirn = std::uniform_int_distribution<> { 0, 2 };
    word_direction = static_cast<direction>(dirn(mtgen));

    curr_word = words.get_longest_word();
    curr_word_size = static_cast<int>(curr_word.size());

    auto rd = std::uniform_int_distribution<> { 0, grid_size-curr_word_size };
    auto cd = std::uniform_int_distribution<> { 0, grid_size-curr_word_size };
    col = cd(mtgen);
    row = rd(mtgen);

    place_word(word_grid, col, row, word_direction, curr_word);
    std::cerr << "First word (longest):" << std::endl;
    words.print_placement(words.get_longest_word_index(), col, row, static_cast<int>(word_direction));

    for (int i = 0; i < words.get_word_count(); i++)
    {
        if ( i != words.get_longest_word_index() )
        {
            curr_word = words.get_word(i);
            available_slots = get_slots(word_grid, grid_size, curr_word);

            if ( available_slots.empty() )
                words.print_placement(i, -1, 0, 0);
            else
            {
                int slot_size = static_cast<int>(available_slots.size());
                if ( slot_size == 1 )
                    slot_no = 0;
                else
                {
                    auto slot = std::uniform_int_distribution<> { 0, slot_size-1 };
                    slot_no = slot(mtgen);
                }

                col = available_slots[slot_no][0];
                row = available_slots[slot_no][1];

                place_word(word_grid, col, row, static_cast<direction>(available_slots[slot_no][2]), curr_word);
                words.print_placement(i, col, row, available_slots[slot_no][2]);
            }
        }
    }
    std::cerr << std::endl;
    std::cerr << "------------------------------------------------";
    std::cerr << std::endl << std::endl;

    //std::vector<int> fill_letters {}; TODO using set of all letters in clean_list
    std::vector<int> fill_letters {65,66,67,68,69,70,71,72};

    fill_blanks(word_grid, grid_size, fill_letters);
    print_grid(word_grid, grid_size, true);

    return 0;
}


std::vector <std::vector<int>> get_slots(std::vector <std::vector<int>> &grid, int grid_size, std::vector<int> &curr_word)
{
    std::vector <std::vector<int>> slots = {};
    bool slot_ok {false};
    std::vector<int> curr_slot = {};
    int word_size {0};

    slots = {};
    word_size = static_cast<int>(curr_word.size());
    for (int i = 0; i < grid_size; i++)
    {
        for (int j = 0; j < grid_size; j++)
        {
            if ( j <= (grid_size - word_size))
            {
                // process ACROSS
                slot_ok = true;
                for (int l = 0; l < word_size; l++)
                {
                    if ( grid[i][j+l] != 0 && grid[i][j+l] != curr_word[l] )
                    {
                        slot_ok = false;
                        break;
                    }
                }
                if ( slot_ok )
                    slots.push_back({j,i,static_cast<int>(direction::ACROSS)});
            }

            if ( i <= (grid_size - word_size))
            {
                // process DOWN
                slot_ok = true;
                for (int l = 0; l < word_size; l++)
                {
                    if ( grid[i+l][j] != 0 && grid[i+l][j] != curr_word[l])
                    {
                        slot_ok = false;
                        break;
                    }
                }
                if ( slot_ok )
                    slots.push_back({j,i,static_cast<int>(direction::DOWN)});
            }

            if ( i <= (grid_size - word_size) && j <= (grid_size - word_size))
            {
                // process DIAG (TODO if level = 3)
                slot_ok = true;
                for (int l = 0; l < word_size; l++)
                {
                    if ( grid[i+l][j+l] != 0 && grid[i+l][j+l] != curr_word[l])
                    {
                        slot_ok = false;
                        break;
                    }
                }
                if ( slot_ok )
                    slots.push_back({j,i,static_cast<int>(direction::DIAG)});
            }
        }
    }

    return slots;
}

bool can_place_word(std::vector <std::vector<int>> &grid, int col, int row, direction word_direction, std::vector<int> &curr_word)
{
    return true;
}

void place_word(std::vector <std::vector<int>> &grid, int col, int row, direction word_direction, std::vector<int> &curr_word)
{
    grid[row][col] = curr_word[0];

    switch (word_direction)
    {
    case direction::ACROSS:
    {
        for(int i=1; i < static_cast<int>(curr_word.size()); i++)
        {
            grid[row][col+i] = curr_word[i];
        }
        break;
    }

    case direction::DOWN:
    {
        for(int i=1; i < static_cast<int>(curr_word.size()); i++)
        {
            grid[row+i][col] = curr_word[i];
        }
        break;
    }

    case direction::DIAG:
    {
        for(int i=1; i < static_cast<int>(curr_word.size()); i++)
        {
            grid[row+i][col+i] = curr_word[i];
        }
        break;
    }
    }
}

void fill_blanks(std::vector <std::vector<int>> &grid, int grid_size, std::vector<int> &fill_chars)
{
    auto ud = std::uniform_int_distribution<> { 0, static_cast<int>(fill_chars.size())-1 };

    for (int i = 0; i < grid_size; i++)
    {
        for (int j = 0; j < grid_size; j++)
        {
            if ( grid[i][j] == 0 )
                grid[i][j] = fill_chars[ud(mtgen)];
        }
    }
}

void print_grid(std::vector <std::vector<int>> &grid, int grid_size, bool as_char)
{
    std::cout << "Word Grid: " << ( as_char ? " (Char)" : " (Int)" ) << std::endl;
    for (int i = 0; i < grid_size; i++)
    {
        for (int j = 0; j < grid_size; j++)
        {
            if ( as_char )
                std::cout << static_cast<char>(grid[i][j]);
            else
                std::cout << static_cast<int>(grid[i][j]);
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}
