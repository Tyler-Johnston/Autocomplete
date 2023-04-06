#include "WordTree.hpp"

TreeNode::TreeNode()
{
}

WordTree::WordTree()
{
}

void WordTree::add(std::string word)
{

    bool createdEndOfWord = false;
    if (word != "")
    {
        std::shared_ptr<TreeNode> v = root;
        for (long unsigned int i = 0; i < word.length(); i++)
        {
            int index = static_cast<int>(static_cast<char>(word[i]) - 97);
            std::shared_ptr<TreeNode> node = std::make_shared<TreeNode>();
            node->endOfWord = false;
            if (v->children[index] == NULL)
            {
                v->children[index] = node;
                if (i == word.length() - 1)
                {
                    v->children[index]->endOfWord = true;
                    createdEndOfWord = true;
                }
            }
            if (v->children[index]->endOfWord && createdEndOfWord)
            {
                WordTree::treeSize++;
            }

            v = v->children[index];
        }
    }
}

bool WordTree::find(std::string word)
{

    std::shared_ptr<TreeNode> v = root;
    for (long unsigned int i = 0; i < word.length(); i++)
    {
        int index = static_cast<int>(static_cast<char>(word[i]) - 97);

        if (v->children[index] == NULL)
        {
            return false;
        }
        else if ((i == word.length() - 1) && (v->children[index]->endOfWord))
        {
            return true;
        }
        v = v->children[index];
    }
    return false;
}

std::vector<std::string> WordTree::predict(std::string partial, std::uint8_t howMany)
{

    std::vector<std::string> predictions;
    std::vector<std::shared_ptr<TreeNode>> explored;
    if (partial != "")
    {
        // traverse until you get to the end of the 'partial' word
        std::shared_ptr<TreeNode> v = root;
        for (long unsigned int i = 0; i < partial.length(); i++)
        {
            int index = static_cast<int>(static_cast<char>(partial[i]) - 97);
            if (v->children[index] != NULL)
            {
                v = v->children[index];
            }
        }

        // implement the BFS algorithm to find each word that needs to be predicted
        uint8_t discoveredWords = 0;
        std::queue<std::tuple<std::string, std::shared_ptr<TreeNode>>> q;
        explored.push_back(v);
        q.push(std::make_tuple(partial, v));
        while (!q.empty())
        {
            std::tuple element = q.front();
            q.pop();
            std::string newPartial = std::get<0>(element);
            v = std::get<1>(element);

            // stop searching if you found the max number of words needed
            if (discoveredWords == howMany)
            {
                break;
            }

            // visit the layer associated with the node, add them to the queue, and if a word is found, add to predictions vector
            for (long unsigned int i = 0; i < v->children.size(); i++)
            {
                auto isExplored = std::find(explored.begin(), explored.end(), v->children[i]);
                if (v->children[i] != NULL && (isExplored == explored.end()))
                {
                    char letter = static_cast<char>(i + 97);
                    std::string str(1, letter);
                    explored.push_back(v->children[i]);
                    q.push(std::make_tuple(newPartial + letter, v->children[i]));
                    if (v->children[i]->endOfWord)
                    {
                        predictions.push_back(newPartial + letter);
                        discoveredWords++;
                    }
                }
            }
        }
    }
    return predictions;
}

std::size_t WordTree::size()
{

    return WordTree::treeSize;
}
