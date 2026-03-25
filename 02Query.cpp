#include "02Query.h";

Query::Query() {}

Table* Query::runQuery() {
    Crud* crud = Crud::getInstance();

    if (action == "create") {
        crud->createTable(destination, source, types);

        return nullptr;
    } else if (action == "addrow") {
        crud->addRow(destination, source);

        return nullptr;
    } else if (action == "delrow") {
        crud->deleteRow(destination, source[0]);

        return nullptr;
    } else if (action == "read") {
        return crud->readTable(destination, source, where, orderby);
    }

    return nullptr;
}