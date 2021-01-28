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

int Word_Grid::read_and_clean_word_list(std::string file_name)
{
    //Process the raw list of words in text file (file_name), clean (remove whitespace) and upper
    std::ifstream file(file_name);

    //Return values: 0=Success, 1=File not found, 2=No valid clean words found
    int result {0};

    if (file.is_open())
    {
        std::string line;
        std::vector<std::string> raw_list {};


        while (std::getline(file, line))
        {
            raw_list.push_back(line);
        }
        file.close();

        bool found_word {false};

        //Process the raw list of words, clean (remove whitespace) and upper
        for (std::string raw_word : raw_list)
        {
            std::string curr_word {};
            found_word = false;

            for (char letter: raw_word)
                if ( isalpha(letter) )
                    curr_word += toupper(letter);

            // Check to see if word already in clean list
            for (auto word: clean_list)
            {
                if ( word->get_word() == curr_word )
                {
                    found_word = true;
                    std::cerr << curr_word << " - Duplicate word: " << std::endl;
                    break;
                }
            }
            if ( ! found_word )
                if ( curr_word.size() > 1 )
                {
                    auto tmp = std::make_shared<word_data>(word_data(curr_word));
                    clean_list.push_back(tmp);
                }
        }
        word_count = static_cast<int>(clean_list.size());

        if ( word_count == 0 )
        {
            std::cerr << std::endl << "No valid words in file" << std::endl;
            result = 2;
        }

        else
            result = 0;
    }
    else
    {
        std::cerr << std::endl << "File not found (" << file_name << ")" << std::endl;
        result = 1;
    }

    return result;
}

bool Word_Grid::create_grid()
{
    // ------------------------------------------------------------------
    // Set-up word grid based on longest word and pre-populate with zeros
    // ------------------------------------------------------------------
    std::uniform_int_distribution<int> filler(2, 7);
    int length {0};
    int longest_word_length {0};

    for (auto word: clean_list)
    {
        length = word->get_letter_count();
        longest_word_length = (length > longest_word_length) ? length : longest_word_length;
    }

    if ( longest_word_length > 0 )
    {
        grid_size = longest_word_length + filler(_rnd);

        word_grid = {};
        word_grid.resize(grid_size, std::vector<int>(grid_size, 0));
        //std::cerr << std::endl << "Longest word = " <<  longest_word_length << std::endl;
        return true;
    }
    else
    {
        std::cerr << std::endl << "Longest word = 0" << std::endl;
        return false;
    }
}

bool Word_Grid::place_all_words()
{
    int result {0};
    bool use_slots {false};

    for (auto curr_word: clean_list)
    {
        result = place_word(curr_word, use_slots);
        use_slots = true;

        if ( result == -1 )
            return false;
    }
    return true;
}

int Word_Grid::place_word(std::shared_ptr<word_data> curr_word_data, bool use_slots)
{
    int col {0};
    int row {0};
    word_data::direction word_direction {};
    std::string curr_word = curr_word_data->get_word();

    if ( use_slots )
    {
        int slot_no {0};

        available_slots = get_slots(curr_word);

        if ( available_slots.empty() )
            return -1;
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
            word_direction = static_cast<word_data::direction>(available_slots[slot_no][2]);
        }
    }
    else
    {
        int curr_word_size = curr_word_data->get_letter_count();

        std::uniform_int_distribution<int> cd(0, grid_size-curr_word_size);
        std::uniform_int_distribution<int> rd(0, grid_size-curr_word_size);
        col = cd(_rnd);
        row = rd(_rnd);

        std::uniform_int_distribution<int> dirn(0, 2);
        word_direction = static_cast<word_data::direction>(dirn(_rnd));
    }

    word_grid[row][col] = curr_word[0];

    switch (word_direction)
    {
        case word_data::direction::ACROSS:
        {
            for(int i=1; i < static_cast<int>(curr_word.size()); i++)
            {
                word_grid[row][col+i] = curr_word[i];
            }
            break;
        }

        case word_data::direction::DOWN:
        {
            for(int i=1; i < static_cast<int>(curr_word.size()); i++)
            {
                word_grid[row+i][col] = curr_word[i];
            }
            break;
        }

        case word_data::direction::DIAG:
        {
            for(int i=1; i < static_cast<int>(curr_word.size()); i++)
            {
                word_grid[row+i][col+i] = curr_word[i];
            }
            break;
        }
    }
    curr_word_data->set_placement(row, col, word_direction);

    return 0;
}

std::vector <std::vector<int>> Word_Grid::get_slots(const std::string &curr_word)
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
                    slots.push_back({j,i,static_cast<int>(word_data::direction::ACROSS)});
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
                    slots.push_back({j,i,static_cast<int>(word_data::direction::DOWN)});
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
                    slots.push_back({j,i,static_cast<int>(word_data::direction::DIAG)});
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

void Word_Grid::print_all_placements()
{
    std::cout << std::endl;

    for (auto curr_word_data: clean_list)
        curr_word_data->print_placement();
}
