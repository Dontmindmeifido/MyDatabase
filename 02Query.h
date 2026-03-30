#include "01Crud.h";
// Split query into different structs, and then based on the parameters, make represent cast to the respective query, then run the query 
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