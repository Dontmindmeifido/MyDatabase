#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include "Database.h"

// Here simply make a strip method, it will modularize everything nicely


class Parser {
public:
    std::string lower(std::string value);

    // Strips all newlines and spaces;
    std::string strip(std::string value);
    
    // For syntax coloring
    std::vector<std::string> getSpacedWords(std::string queries);

    // For getSpacedWords
    std::vector<std::string> partitionQuery(std::string value);

    // For name suggestions
    std::vector<std::string> getSnippet(std::string lastWord, Database& db);

    // For name suggestions rect positioning
    std::pair<int, int> getCursorPosition(std::string queryBuffer);
};