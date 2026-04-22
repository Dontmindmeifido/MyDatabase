#pragma once
#include "../Database/Database.h"

class Update {
public:
    static void run(Database* database, std::string tableName, std::vector<std::string> row);
};