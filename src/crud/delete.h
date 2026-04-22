#pragma once
#include "../Database/Database.h"

class Delete {
public:
    static void run(Database* database, std::string tableName, std::string row);
};