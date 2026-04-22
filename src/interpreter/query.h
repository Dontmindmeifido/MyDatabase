#pragma once
#include "../Crud/Create.h"
#include "../Crud/Read.h"
#include "../Crud/Update.h"
#include "../Crud/Delete.h"

struct Query {
    std::string action;
    std::vector<std::string> actionParameters0;
    std::vector<std::string> actionParameters1;
    std::string table;
    std::vector<std::string> filterWhereParameters;
    std::vector<std::string> filterOrderByParameters;

    Query();

    Table* runQuery(Database* database);
};