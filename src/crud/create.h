#pragma once
#include "../Database/Database.h"

class Create {
public:
    static void run(Database* database, std::string tableName, std::vector<std::string> headers, std::vector<std::string> dataTypes);
};