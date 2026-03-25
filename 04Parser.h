#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include "00Database.h"

class Parser {
public:
    void toUpper(std::string& value);

    std::string retToLower(std::string value);

    // Strips all newlines and spaces;
    std::string retStrip(std::string value);
    
    // Removes spaces inside paranthesis of queries
    void removeSpaces(std::string& value);

    // Removes in keywords in list of token lists
    void removeIn(std::vector<std::vector<std::string>>& values);

    // Splits instructions by the separator ';'
    std::vector<std::string> splitInstructions(std::string value);

    // Tokenizes general arguments (separated only by comma, undefined number of inputs)
    std::vector<std::string> tokenizeGeneralArgument(std::string value);

    // Tokenizes filter arguments (separated only by comma, 2 or 3 inputs)
    std::vector<std::string> tokenizeFilterArgument(std::string value);

    // Tokenizes Create crud arguments (separated by comma and double point)
    std::pair<std::vector<std::string>, std::vector<std::string>> tokenizeCreateArgument(std::vector<std::string> header);

    // For syntax coloring
    std::vector<std::string> getSpacedWords(std::string queries);

    // For getSpacedWords
    std::vector<std::string> partitionQuery(std::string value);

    // For name suggestions
    std::vector<std::string> getSnippet(std::string lastWord, Database& db);

    // For name suggestions rect positioning
    std::pair<int, int> getCursorPosition(std::string queryBuffer);
};