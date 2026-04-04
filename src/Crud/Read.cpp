#include "Read.h"

Table* Read::run(std::string tableName, std::vector<std::string> headers, std::vector<std::string> where, std::vector<std::string> orderby) {
    Table* table = this->database->getTable(tableName);

    if (table == nullptr || headers.empty() || where.size() % 3 != 0 || orderby.size() % 2 != 0) return nullptr; // miss empty arguments
    
    // Get columns
    std::vector<Row> dummyrows = {Row(headers)};
    for (int i = 1; i < (int)table->getTableData().size(); i++) {
        int column = 0;
        std::vector<std::string> rowdata;

        for (int j = 0; j < (int)table->getTableData()[i].getRowData().size(); j++) { // Direction sensitive
            if (table->getTableData()[0].getRowData()[j].getValue() == headers[column]) {
                rowdata.push_back(table->getTableData()[i].getRowData()[j].getValue());

                if (column < (int)headers.size() - 1) {
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
        for (int i = 0; i < (int)table->getTableData()[0].getRowData().size(); i++) {
            if (table->getTableData()[0].getRowData()[i].getValue() == column) {
                columnIndex = i;
            }
        }

        for (int i = 1; i < (int)dummyrows.size(); i++) {
            if (operation == ">") {
                if (std::stoi(table->getTableData()[i].getRowData()[columnIndex].getValue()) > std::stoi(comparator)) {
                    rowdata.push_back(dummyrows[i]);
                }
            } else if (operation == "<") {
                if (std::stoi(table->getTableData()[i].getRowData()[columnIndex].getValue()) < std::stoi(comparator)) {
                    rowdata.push_back(dummyrows[i]);
                }
            } else if (operation == ">=") {
                if (std::stoi(table->getTableData()[i].getRowData()[columnIndex].getValue()) >= std::stoi(comparator)) {
                    rowdata.push_back(dummyrows[i]);
                }
            } else if (operation == "<=") {
                if (std::stoi(table->getTableData()[i].getRowData()[columnIndex].getValue()) <= std::stoi(comparator)) {
                    rowdata.push_back(dummyrows[i]);
                }
            } else if (operation == "=") {
                if (std::stoi(table->getTableData()[i].getRowData()[columnIndex].getValue()) == std::stoi(comparator)) {
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
        for (int i = 0; i < (int)headers.size(); i++) {
            if (headers[i] == column) {
                columnIndex = i;
            }
        }

        if (order == "descending") {
            sort(dummyrows.begin() + 1, dummyrows.end(), [&columnIndex](Row a, Row b) {return std::stoi(a.getRowData()[columnIndex].getValue()) > std::stoi(b.getRowData()[columnIndex].getValue());});
        } else if (order == "ascending"){
            sort(dummyrows.begin() + 1, dummyrows.end(), [&columnIndex](Row a, Row b) {return std::stoi(a.getRowData()[columnIndex].getValue()) < std::stoi(b.getRowData()[columnIndex].getValue());});
        }
    }

    Table* retTable = new Table(headers, {"ANY"}, "RESPONSE");
    retTable->setTableData(dummyrows);

    return retTable;
}