#include "Crud.h";

struct Query {
    std::string action;
    std::vector<std::string> actionParameters0;
    std::vector<std::string> actionParameters1;
    std::string table;
    std::vector<std::string> filterWhereParameters;
    std::vector<std::string> filterOrderByParameters;

    Query();

    Table* runQuery();
};