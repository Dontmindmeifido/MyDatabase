#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include "../database/database.h"

class Parser {
public:
    std::string get_lower(std::string value);
    std::string get_stripped(std::string value);
    std::vector<std::string> get_spaced_words(std::string queries);
    std::vector<std::string> get_partitioned_query(std::string value);
    std::vector<std::string> get_snippets(std::string lastWord, Database& db);
    std::pair<int, int> get_cursor_position(std::string queryBuffer);
};