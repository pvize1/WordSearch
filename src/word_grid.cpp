#include "word_grid.h"

Word_Grid::Word_Grid()
    : Word_Grid {0}
{
    //ctor
}

Word_Grid::Word_Grid(int set_level)
    : level { set_level }
{
    //ctor
}

Word_Grid::~Word_Grid()
{
    //dtor
}

bool Word_Grid::read_and_clean_word_list(std::string filename)
{
    bool read_ok {false};

    //Process the raw list of words, clean (remove whitespace) and upper
    file_name = filename;

    switch ( words.create_clean_list(filename) )
    {
        case 0:
        {
            read_ok = true;
            break;
        }
        case 1:
        {
            std::cerr << std::endl << "File not found (" << file_name << ")" << std::endl;
            break;
        }
        case 2:
        {
            std::cerr << std::endl << "No valid words in file" << std::endl;
            break;
        }
    }
    return read_ok;
}

bool Word_Grid::create_grid()
{
    // ------------------------------------------------------------------
    // Set-up word grid based on longest word and pre-populate with zeros
    // ------------------------------------------------------------------
    std::uniform_int_distribution<int> filler(2, 7);
    int longest_word {};

    longest_word = words.get_longest_word_length();

    if ( longest_word > 0 )
    {
        grid_size = longest_word + filler(_rnd);

        word_grid = {};
        word_grid.resize(grid_size, std::vector<int>(grid_size, 0));
        return true;
    }
    else
    {
        std::cerr << std::endl << "Longest word = 0" << std::endl;
        return false;
    }
}

int Word_Grid::get_grid_size()
{
    return grid_size;
}

bool Word_Grid::place_all_words()
{
    std::vector<int> curr_word {};
    std::vector<int> placement {};

    //placement = place_word(words.get_longest_word(), false);
    placement = place_word(words.get_word(0), false);

    for (int i = 1; i < words.get_word_count(); i++)
    {
        //if ( i != words.get_longest_word_index() )
        //{
            curr_word = words.get_word(i);
            placement = place_word(curr_word, true);
        //}
        if ( placement[0] == -1 )
            return false;
    }
    return true;
}

std::vector<int> Word_Grid::place_word(std::vector<int> curr_word, bool use_slots)
{
    int col {0};
    int row {0};

    if ( use_slots )
    {
        int slot_no {0};

        available_slots = get_slots(curr_word);

        if ( available_slots.empty() )
        {
            print_placement(curr_word, -1, 0, 0);
            return {-1, 0, 0};
        }
        else
        {
            int slot_size = static_cast<int>(available_slots.size());

            if ( slot_size == 1 )
                slot_no = 0;
            else
            {
                auto slot = std::uniform_int_distribution<> { 0, slot_size-1 };
                slot_no = slot(_rnd);
            }

            col = available_slots[slot_no][0];
            row = available_slots[slot_no][1];
            word_direction = static_cast<direction>(available_slots[slot_no][2]);
        }
    }
    else
    {
        int curr_word_size {0};
        std::uniform_int_distribution<int> dirn(0, 2);

        word_direction = static_cast<direction>(dirn(_rnd));
        curr_word_size = static_cast<int>(curr_word.size());

        std::uniform_int_distribution<int> cd(0, grid_size-curr_word_size);
        std::uniform_int_distribution<int> rd(0, grid_size-curr_word_size);

        col = cd(_rnd);
        row = rd(_rnd);
    }

    word_grid[row][col] = curr_word[0];

    switch (word_direction)
    {
        case direction::ACROSS:
        {
            for(int i=1; i < static_cast<int>(curr_word.size()); i++)
            {
                word_grid[row][col+i] = curr_word[i];
            }
            break;
        }

        case direction::DOWN:
        {
            for(int i=1; i < static_cast<int>(curr_word.size()); i++)
            {
                word_grid[row+i][col] = curr_word[i];
            }
            break;
        }

        case direction::DIAG:
        {
            for(int i=1; i < static_cast<int>(curr_word.size()); i++)
            {
                word_grid[row+i][col+i] = curr_word[i];
            }
            break;
        }
    }
    print_placement(curr_word, col, row, static_cast<int>(word_direction));
    return { col, row, static_cast<int>(word_direction) };
}

void Word_Grid::print_placement(std::vector<int> &curr_word, int col, int row, int direction)
{
    for (auto letter: curr_word)
        std::cerr << static_cast<char>(letter);
    if ( col == -1 )
        std::cerr << "\t\t NOT PLACED" << std::endl;
    else
    {
        std::cerr << "\t\t Placed at:row=" << row << ",col=" << col;
        std::cerr << ",Direction=" << direction << std::endl;
    }
}

std::vector <std::vector<int>> Word_Grid::get_slots(std::vector<int> &curr_word)
{
    std::vector <std::vector<int>> slots {};
    bool slot_ok {false};
    std::vector<int> curr_slot {};
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
                    if ( word_grid[i][j+l] != 0 && word_grid[i][j+l] != curr_word[l] )
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
                    if ( word_grid[i+l][j] != 0 && word_grid[i+l][j] != curr_word[l])
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
                    if ( word_grid[i+l][j+l] != 0 && word_grid[i+l][j+l] != curr_word[l])
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

void Word_Grid::fill_blanks(std::vector<int> &fill_chars)
{
    std::uniform_int_distribution<int> idx(0, static_cast<int>(fill_chars.size())-1);

    for (int i = 0; i < grid_size; i++)
    {
        for (int j = 0; j < grid_size; j++)
        {
            if ( word_grid[i][j] == 0 )
                word_grid[i][j] = fill_chars[idx(_rnd)];
        }
    }
}

void Word_Grid::print_grid(bool as_char)
{
    std::cout << std::endl << "Word Grid: " << ( as_char ? " (Char)" : " (Int)" ) << std::endl;
    for (int i = 0; i < grid_size; i++)
    {
        for (int j = 0; j < grid_size; j++)
        {
            if ( as_char )
                std::cout << static_cast<char>(word_grid[i][j]);
            else
                std::cout << static_cast<int>(word_grid[i][j]);
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}
