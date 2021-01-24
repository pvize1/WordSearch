#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<random>


enum struct direction {ACROSS, DOWN, DIAG} word_direction;

void print_word(std::vector<int> &curr_word, bool as_char);
bool is_word_match(std::vector<int> &word_1, std::vector<int> &word_2);
std::vector <std::vector<int>> get_slots(std::vector <std::vector<int>> &grid, std::vector<int> &curr_word);
void place_word(std::vector <std::vector<int>> &grid, int col, int row, direction word_direction, std::vector<int> &curr_word);
void fill_blanks(std::vector <std::vector<int>> &grid, int grid_size, std::vector<int> fill_chars);
void print_grid(std::vector <std::vector<int>> &grid, int grid_size, bool as_char);

std::random_device rd{};
auto mtgen = std::mt19937
{
    rd()
};


int main()
{
    const std::string FILENAME = "C:\\Users\\peter\\c++\\WordSearch\\words.txt";
    std::ifstream file(FILENAME);

    int word_count {0};
    int word_length {0};
    std::vector<std::string> word_list_raw {};
    std::vector<std::vector<int>> word_list_clean {};
    bool found_word {false};
    int longest_word {0};
    std::vector <std::vector<int>> word_grid {};
    std::vector <std::vector<int>> available_slots {};
    std::vector<int> curr_word {0};
    int grid_size {0};
    int row {0};
    int col {0};

    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            word_count++;
            word_list_raw.push_back(line);
            std::cerr << str << std::endl;
        }
        file.close();

        std::cerr << "Count of words=" << word_count << std::endl;
        std::cerr << "Array size=" << static_cast<int>(word_list_raw.size()) << std::endl << std::endl;
        std::cerr << "Word list:" << std::endl;

        for (std::string str : word_list_raw)
        {

        }

        for (int i = 0; i < static_cast<int>(word_list_raw.size()); i++)
        {
            word_length = static_cast<int>(word_list_raw[i].size());
            curr_word = {};
            found_word = false;

            std::cout << std::endl;

            for (int j = 0; j < word_length; j++)
            {
                std::cout << static_cast<int>(toupper(word_list_raw[i][j])) << " ";

                if ( isalpha(word_list_raw[i][j]) )
                {
                    curr_word.push_back(static_cast<int>(toupper(word_list_raw[i][j])));
                }
            }

            for (std::vector<int> v_word : word_list_clean)
            {
                if ( is_word_match(curr_word, v_word) )
                {
                    found_word = true;
                    break;
                }
            }

            //found_word = word_list_clean.find(v_word);

            if ( found_word )
            {
                std::cout << " - Duplicate word: ";
                word_count--;
            }
            else
            {
                longest_word = (static_cast<int>(curr_word.size()) > longest_word) ? static_cast<int>(curr_word.size()) : longest_word;
                word_list_clean.push_back(curr_word);
            }
        }

        std::cout << std::endl << "Corrected count of words=" << word_count << std::endl;

        for (int i = 0; i < word_count; i++)
        {
            std::cout << "Letters of (" << i+1 << ") word: ";

            word_length = static_cast<int>(word_list_clean[i].size());
            std::cout << "length=" << word_length << std::endl;
            print_word(word_list_clean[i], true);
        }
        std::cout << std::endl << "Longest length=" << longest_word << std::endl;
        std::cout << "Longest word(s):" << std::endl;

        for (int i = 0; i < static_cast<int>(word_list_clean.size()); i++)
        {
            if ( static_cast<int>(word_list_clean[i].size()) == longest_word )
                print_word(word_list_clean[i], true);
        }

        // -----------------------------------------------------------------
        // Set-up word grid based on longest word and pre-populate with zero
        // -----------------------------------------------------------------
        grid_size = longest_word + 2;
        word_grid.resize(grid_size,std::vector<int>(grid_size, 0));

        word_direction = direction::DIAG;
        std::cout << std::endl << "Direction=" << static_cast<int>(word_direction) << std::endl;
        //std::cout << "Grid Size=" << grid_size << std::endl;
        //std::cout << "Curr Word Size=" << (int)curr_word.size() << std::endl;
        //std::cout << "Distribution=" << grid_size-((int)curr_word.size()) << std::endl;

        curr_word = word_list_clean[0];
        auto rd = std::uniform_int_distribution<> { 0, grid_size-((int)curr_word.size()) };
        auto cd = std::uniform_int_distribution<> { 0, grid_size-((int)curr_word.size()) };

        col = cd(mtgen);
        row = rd(mtgen);

        std::cout << "Placed at: row=" << row << ", col=" << col << std::endl;
        print_word(curr_word, false);
        place_word(word_grid, col, row, word_direction, curr_word);

        //word_grid[ud(mtgen)][ud(mtgen)] = toupper('a');

        for (int i = 1; i < (int)word_list_clean.size(); i++)
        {
            //curr_word = word_list_clean[i];
            //available_slots = get_slots(word_grid, curr_word);
            //place_word(word_grid, cd(mtgen), rd(mtgen), word_direction, curr_word);
        }

        //std::cout << "Function: fill_blanks" << std::endl;
        fill_blanks(word_grid, grid_size, {68,69,70});
        //std::cout << "Function: print_grid" << std::endl;
        print_grid(word_grid, grid_size, true);
    }
}


void print_word(std::vector<int> &curr_word, bool as_char)
{
    for (int i = 0; i < static_cast<int>(curr_word.size()); i++)
    {
        if ( as_char )
        {
            std::cout << static_cast<char>(curr_word[i]);
        }
        else
        {
            std::cout << static_cast<int>(curr_word[i]);
        }
        std::cout << " ";
    }
    std::cout << ( as_char ? " (Char)" : " (Int)" ) << std::endl;
}

bool is_word_match(std::vector<int> &word_1, std::vector<int> &word_2)
{
    if ( word_1.size() != word_2.size() )
    {
        return false;
    }
    else
    {
        for (int i = 0; i < static_cast<int>(word_1.size()); i++)
        {
            if ( word_1[i] != word_2[i] )
            {
                return false;
            }
        }
    }

    return true;
}

std::vector <std::vector<int>> get_slots(std::vector <std::vector<int>> &grid, std::vector<int> &curr_word)
{
    std::vector <std::vector<int>> slots = {};
    return slots;
}

void place_word(std::vector <std::vector<int>> &grid, int col, int row, direction word_direction, std::vector<int> &curr_word)
{
    //std::cout << "Function: place_word" << std::endl;

    grid[row][col] = curr_word[0];
    switch (word_direction)
    {
        case direction::ACROSS:
        {
            std::cout << "ACROSS" << std::endl;
            for(int i=1; i < static_cast<int>(curr_word.size()); i++)
            {
                //std::cout << "Letter=" << i << std::endl;
                grid[row][col+i] = curr_word[i];
                //std::cout << "Placed" << std::endl;
            }
            break;
        }

        case direction::DOWN:
        {
            std::cout << "DOWN" << std::endl;
            for(int i=1; i < static_cast<int>(curr_word.size()); i++)
            {
                grid[row+i][col] = curr_word[i];
            }
            break;
        }

        case direction::DIAG:
        {
            std::cout << "DIAG" << std::endl;
            for(int i=1; i < static_cast<int>(curr_word.size()); i++)
            {
                grid[row+i][col+i] = curr_word[i];
            }
            break;
        }
    }
    //std::cout << "Function: END" << std::endl;
}

void fill_blanks(std::vector <std::vector<int>> &grid, int grid_size, std::vector<int> fill_chars)
{
    auto ud = std::uniform_int_distribution<> { 0, static_cast<int>(fill_chars.size())-1 };

    for (int i = 0; i < grid_size; i++)
    {
        for (int j = 0; j < grid_size; j++)
        {
            if ( grid[i][j] == 0 )
            {
                grid[i][j] = fill_chars[ud(mtgen)];
            }
        }
    }
}

void print_grid(std::vector <std::vector<int>> &grid, int grid_size, bool as_char)
{
    std::cout << std::endl << "Word Grid: " << ( as_char ? " (Char)" : " (Int)" ) << std::endl;
    for (int i = 0; i < grid_size; i++)
    {
        for (int j = 0; j < grid_size; j++)
        {
            if ( as_char )
            {
                std::cout << static_cast<char>(grid[i][j]);
            }
            else
            {
                std::cout << static_cast<int>(grid[i][j]);
            }
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}
