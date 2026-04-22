#pragma once
#include <algorithm>
#include "../Database/Database.h"

class Read {
public:
    static Table* run(Database* database, std::string tableName, std::vector<std::string> headers, std::vector<std::string> where, std::vector<std::string> orderby);
};