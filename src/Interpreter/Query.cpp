#include "Query.h";

Query::Query() {}

Table* Query::runQuery() {
    Crud* crud = Crud::getInstance();

    if (action == "create") {
        crud->Create::run(table, actionParameters0, actionParameters1);

        return nullptr;
    } else if (action == "addrow") {
        crud->Update::run(table, actionParameters0);

        return nullptr;
    } else if (action == "delrow") {
        if (actionParameters0.empty()) {
            crud->Delete::run(table, "");
        } else  {
            crud->Delete::run(table, actionParameters0[0]);
        }

        return nullptr;
    } else if (action == "read") {
        return crud->Read::run(table, actionParameters0, filterWhereParameters, filterOrderByParameters);
    } else {
        return nullptr;
    }
}