#include "01Crud.h"


Connection::Connection() {
    this->database = Database::getInstance();
}

Connection::~Connection() {
    delete database;
}

const Database* Connection::getConnection() {
    return this->database;
}


Create::~Create() {}

void Create::createTable(std::string tableName, std::vector<std::string> headers, std::vector<std::string> dataTypes) {
    Table* table = this->database->getTable(tableName);

    if (table != nullptr || headers.empty() || headers.empty()) return;

    std::vector<Table> databaseData = this->database->getDatabaseData();
    databaseData.push_back(Table(headers, dataTypes, tableName));

    this->database->setDatabaseData(databaseData);
}


Table* Read::readTable(std::string tableName, std::vector<std::string> headers, std::vector<std::string> where, std::vector<std::string> orderby) {
    Table* table = this->database->getTable(tableName);

    if (table == nullptr || headers.empty() || where.size() % 3 != 0 || orderby.size() % 2 != 0) return nullptr; // miss empty arguments

    // Get columns
    std::vector<Row> dummyrows = {Row(headers)};
    for (int i = 1; i < table->getTableData().size(); i++) {
        int column = 0;
        std::vector<std::string> rowdata;

        for (int j = 0; j < table->getTableData()[i].getRowData().size(); j++) { // Direction sensitive
            if (table->getTableData()[0].getRowData()[j].getValue() == headers[column]) {
                rowdata.push_back(table->getTableData()[i].getRowData()[j].getValue());

                if (column < headers.size() - 1) {
                    column++;
                }
            }
        }

        if (!rowdata.empty()) {
            dummyrows.push_back(Row(rowdata));
        }
    }

    // Where Filter
    if (!where.empty()) {
        std::vector<Row> rowdata = {Row(headers)};

        std::string column = where[0];
        std::string operation = where[1];
        std::string comparator = where[2];

        int columnIndex = 0;
        for (int i = 0; i < table->getTableData()[0].getRowData().size(); i++) {
            if (table->getTableData()[0].getRowData()[i].getValue() == column) {
                columnIndex = i;
            }
        }

        for (int i = 1; i < dummyrows.size(); i++) {
            if (operation == ">") {
                if (table->getTableData()[i].getRowData()[columnIndex].getValue() > comparator) {
                    rowdata.push_back(dummyrows[i]);
                }
            } else if (operation == "<") {
                if (table->getTableData()[i].getRowData()[columnIndex].getValue() < comparator) {
                    rowdata.push_back(dummyrows[i]);
                }
            } else if (operation == ">=") {
                if (table->getTableData()[i].getRowData()[columnIndex].getValue() >= comparator) {
                    rowdata.push_back(dummyrows[i]);
                }
            } else if (operation == "<=") {
                if (table->getTableData()[i].getRowData()[columnIndex].getValue() <= comparator) {
                    rowdata.push_back(dummyrows[i]);
                }
            } else if (operation == "=") {
                if (table->getTableData()[i].getRowData()[columnIndex].getValue() == comparator) {
                    rowdata.push_back(dummyrows[i]);
                }
            }

        }

        dummyrows = rowdata;
    }

    // Orderby Filter
    if (!orderby.empty()) {
        std::string column = orderby[0];
        std::string order = orderby[1];

        int columnIndex = 0;
        for (int i = 0; i < headers.size(); i++) {
            if (headers[i] == column) {
                columnIndex = i;
            }
        }

        if (order == "descending") {
            sort(dummyrows.begin() + 1, dummyrows.end(), [&columnIndex](Row a, Row b) {return a.getRowData()[columnIndex].getValue() > b.getRowData()[columnIndex].getValue();});
        } else if (order == "ascending"){
            sort(dummyrows.begin() + 1, dummyrows.end(), [&columnIndex](Row a, Row b) {return a.getRowData()[columnIndex].getValue() < b.getRowData()[columnIndex].getValue();});
        }
    }

    Table* retTable = new Table(headers, {"ANY"}, "RESPONSE");
    retTable->setTableData(dummyrows);

    return retTable;
}


void Update::addRow(std::string tableName, std::vector<std::string> row) {
    Table* table = this->database->getTable(tableName);

    if (table == nullptr || row.empty()) return;

    std::vector<Row> tableData = table->getTableData();
    tableData.push_back(Row(row));

    table->setTableData(tableData);
}


void Delete::deleteRow(std::string tableName, std::string row) {
    Table* table = this->database->getTable(tableName);

    if (table == nullptr) return;

    if (row.empty()) row = "0";

    if (table->getTableData().size() >= 2) {
        std::vector<Row> dummy;
        for (int i = 0; i < table->getTableData().size(); i++) {
            if (i == stoi(row)) {
                continue;
            }

            dummy.push_back(table->getTableData()[i]);
        }

        table->setTableData(dummy);
    }
}

Crud::Crud() {}

Crud* Crud::getInstance() {
    if (!instance) instance = new Crud();
    return instance;
}


Crud* Crud::instance = nullptr;

