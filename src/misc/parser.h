#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include "../Database/Database.h"

class Parser {
public:
    std::string lower(std::string value);
    std::string strip(std::string value);
    std::vector<std::string> getSpacedWords(std::string queries);
    std::vector<std::string> partitionQuery(std::string value);
    std::vector<std::string> getSnippet(std::string lastWord, Database& db);
    std::pair<int, int> getCursorPosition(std::string queryBuffer);
};