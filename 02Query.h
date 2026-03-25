#include "01Crud.h";

struct Query {
    std::string action;
    std::vector<std::string> source;
    std::vector<std::string> types;
    std::string destination;
    std::vector<std::string> where;
    std::vector<std::string> orderby;

    Query();

    Table* runQuery();
};