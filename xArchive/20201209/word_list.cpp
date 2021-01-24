#include "word_list.h"

Word_List::Word_List()
{
    //ctor
}

Word_List::~Word_List()
{
    //dtor
}

bool Word_List::read_raw_list(std::string file_name)
{
    int word_count {0};
    std::ifstream file(file_name);

    if (file.is_open())
    {
        std::string line;
        std::cerr << "Word list:" << std::endl;

        while (std::getline(file, line))
        {
            word_count++;
            raw_list.push_back(line);
            std::cerr << line << std::endl;
        }
        file.close();

        std::cerr << std::endl;
        std::cerr << "Count of words=" << word_count << std::endl;
        std::cerr << "Array size=" << static_cast<int>(raw_list.size());
        std::cerr << std::endl << std::endl;
        std::cerr << "------------------------------------------------";
        std::cerr << std::endl;

        return true;
    }
    else
    {
        return false;
    }
}

void Word_List::create_clean_list()
{
    std::vector<int> curr_word {0};
    bool found_word {false};
    int char_as_int {0};


    //Process the raw list of words, clean (remove whitespace) and upper
    for (auto raw_word : raw_list)
    {
        curr_word = {};
        found_word = false;

        std::cerr << std::endl;

        for (auto letter : raw_word)
        {
            char_as_int = toupper(letter);
            std::cerr << char_as_int << " ";

            if ( isalpha(letter) )
                curr_word.push_back(char_as_int);
        }

        // Check to see if word already in clean list
        for (std::vector<int> vect_word : clean_list)
        {
            if ( is_word_match(curr_word, vect_word) )
            {
                found_word = true;
                std::cerr << " - Duplicate word: ";
                break;
            }
        }
        if ( ! found_word )
            if ( static_cast<int>(curr_word.size()) > 1 )
                clean_list.push_back(curr_word);
    }
}

bool Word_List::is_word_match(std::vector<int> &word_1, std::vector<int> &word_2)
{
    if ( word_1.size() != word_2.size() )
        return false;
    else
    {
        for (int i = 0; i < static_cast<int>(word_1.size()); i++)
        {
            if ( word_1[i] != word_2[i] )
                return false;
        }
    }
    return true;
}

void Word_List::print_word_list()
{
    word_count = static_cast<int>(clean_list.size());

    std::cerr << std::endl;
    std::cerr << std::endl << "Corrected count of words=" << word_count;
    std::cerr << std::endl << std::endl;
    std::cerr << "------------------------------------------------";
    std::cerr << std::endl << std::endl;

    int i {0};
    int word_length {0};

    for (auto curr_word : clean_list)
    {
        std::cerr << "Letters of word (" << i+1 << "): ";

        word_length = static_cast<int>(curr_word.size());
        longest_word_length = (word_length > longest_word_length) ? word_length : longest_word_length;

        std::cerr << "length=" << word_length << std::endl;
        print_word(i, true);
        i++;
    }

    std::cerr << std::endl << "Longest length=" << longest_word_length << std::endl;
    std::cerr << "Longest word(s):" << std::endl;
    longest_word_index = 99;

    for (int i = 0; i < word_count; i++)
    {
        if ( static_cast<int>(clean_list[i].size()) == longest_word_length )
        {
            print_word(i, true);
            longest_word_index = (i > longest_word_index) ? longest_word_index : i;
        }
    }
    std::cerr << std::endl << std::endl;
}

void Word_List::print_word(int word_index, bool as_char)
{
    //for (int i = 0; i < static_cast<int>(curr_word.size()); i++)
    for (auto letter: clean_list[word_index])
    {
        if ( as_char )
            std::cerr << static_cast<char>(letter);
        else
            std::cerr << static_cast<int>(letter);
        std::cerr << " ";
    }
    std::cerr << ( as_char ? " (Char)" : " (Int)" ) << std::endl;
}

void Word_List::print_placement(int word_index, int col, int row, int direction)
{
    for (auto letter: clean_list[word_index])
        std::cerr << static_cast<char>(letter);
    if ( col == -1 )
        std::cerr << "\t\t NOT PLACED" << std::endl;
    else
    {
        std::cerr << "\t\t Placed at: row=" << row << ", col=" << col;
        std::cerr << ", Direction=" << direction << std::endl;
    }
}

std::vector<int> Word_List::get_longest_word()
{
  return (clean_list[longest_word_index]);
}

int Word_List::get_longest_word_length()
{
    return (longest_word_length);
}

int Word_List::get_word_count()
{
   return (word_count);
}

int Word_List::get_longest_word_index()
{
  return (longest_word_index);
}

std::vector<int> Word_List::get_word(int list_index)
{
  return (clean_list[list_index]);
}
