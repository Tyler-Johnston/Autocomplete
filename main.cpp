#include "WordTree.hpp"

std::shared_ptr<WordTree> readDictionary(std::string filename)
{
    auto wordTree = std::make_shared<WordTree>();
    std::ifstream inFile = std::ifstream(filename, std::ios::in);

    while (!inFile.eof())
    {
        std::string word;
        std::getline(inFile, word);
        // Need to consume the carriage return character for some systems, if it exists
        if (!word.empty() && word[word.size() - 1] == '\r')
        {
            word.erase(word.end() - 1);
        }
        // Keep only if everything is an alphabetic character -- Have to send isalpha an unsigned char or
        // it will throw exception on negative values; e.g., characters with accent marks.
        if (std::all_of(word.begin(), word.end(), [](unsigned char c)
                        { return std::isalpha(c); }))
        {
            std::transform(word.begin(), word.end(), word.begin(), [](char c)
                           { return static_cast<char>(std::tolower(c)); });
            wordTree->add(word);
        }
    }

    return wordTree;
}

// code from https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
template <typename Out>
void split(const std::string& s, char delim, Out result)
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim))
    {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

int main()
{
    auto wordtree = readDictionary("dictionary.txt");

    rlutil::saveDefaultColor();
    rlutil::cls();
    rlutil::hidecursor();

    std::string sentence = "";
    while (true)
    {
        if (kbhit())
        {
            int input = rlutil::getkey();
            input = std::tolower(input);
            if (input != rlutil::KEY_ENTER)
            {
                char convertedInput = static_cast<char>(input);
                if (input != rlutil::KEY_BACKSPACE)
                {
                    sentence += convertedInput;
                }
                if (input == rlutil::KEY_ESCAPE)
                {
                    break;
                }
                if (input == rlutil::KEY_BACKSPACE)
                {
                    sentence = sentence.substr(0, sentence.size() - 1);
                }

                rlutil::cls();
                std::cout << sentence << std::endl;
                std::cout << "\n --- prediction ---" << std::endl;
                if (sentence.size() > 0)
                {
                    std::vector<std::string> wordVector = split(sentence, ' ');
                    std::string lastWord = wordVector[wordVector.size() - 1];

                    auto words = wordtree->predict(lastWord, 10);
                    for (unsigned long int i = 0; i < words.size(); i++)
                    {
                        std::cout << words[i] << std::endl;
                    }
                }
            }
        }
    }

    rlutil::showcursor();
    rlutil::resetColor();

    return 0;
}
