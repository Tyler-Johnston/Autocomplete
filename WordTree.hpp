#pragma once
#include "rlutil.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
class TreeNode
{
  public:
    TreeNode();

    bool endOfWord;
    std::array<std::shared_ptr<TreeNode>, 26> children;
};

class WordTree
{
  public:
    WordTree();

    void add(std::string word);
    bool find(std::string word);
    std::vector<std::string> predict(std::string partial, std::uint8_t howMany);
    std::size_t size();

  private:
    int treeSize = 0;
    std::shared_ptr<TreeNode> root = std::make_shared<TreeNode>();
};
