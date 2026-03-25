#include "01Crud.cpp";

struct Query {
    string action;
    vector<string> source;
    string destination;
    vector<string> where;
    vector<string> orderby;

    Query() { }

    Table runQuery() {
        Crud* crud = Crud::getInstance();

        if (action == "create") {
            crud->createTable(destination, source);
            return *(crud->getConnection()->getNullTable());
        } else if (action == "addrow") {
            crud->addRow(destination, source);
            return *(crud->getConnection()->getNullTable());
        } else if (action == "delrow") {
            if (source[0] == "") {
                crud->deleteRow(destination);
            } else {
                crud->deleteRow(destination, stoi(source[0]));
            }

            return *(crud->getConnection()->getNullTable());
        } else if (action == "read") {
            return crud->readTable(destination, source, where, orderby);
        }
    }
};