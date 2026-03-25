#include <iostream>
#include <vector>
#include <algorithm>
#include "00Database.cpp"

using namespace std;

class Connection {
protected:
    Database* database;

    Connection() {
        this->database = Database::getInstance();
    }

public:
    Database* getConnection() {
        return this->database;
    }

    virtual ~Connection() {
        delete database;
    }
};

class Create : virtual public Connection {
public:
    void createTable(string tableName, vector<string> header) {
        if (&(this->database->getTable(tableName)) == this->database->getNullTable()) {
            vector<string> headers;
            vector<string> datatypes;

            for (int i = 0; i < header.size(); i++) {
                bool side = true;
                headers.push_back("");
                datatypes.push_back("");
                
                for (auto x: header[i]) {
                    if (x == ':') {
                        side = false;
                    } else {
                        if (side) {
                            headers[headers.size() - 1] += x;
                        } else {
                            datatypes[datatypes.size() - 1] += x;
                        }
                    }
                }
            }

            this->database->getDatabaseData().push_back(Table(headers, datatypes, tableName));
        } else {
            cout << "TABLE ALREADY EXISTS";
        }
    }
};

class Read : virtual public Connection {
public:
    Table readTable(string tableName, vector<string> headers, vector<string> where, vector<string> orderby) { // TODO add where stuff, order by, etc..
        Table& table = this->database->getTable(tableName);

        Table retTable(headers, {""}, "RESPONSE");
        vector<Row> dummyrows = {Row(headers)};
        
        string column, operation, comparator, columnOrder, comparatorOrder;
        int columnIndex, columnIndexOrder;

        if (!where.empty()) {
            column = where[0];
            operation = where[1];
            comparator = where[2];

            columnIndex = 0;
            for (int i = 0; i < table.getTableData()[0].getRowData().size(); i++) {
                if (table.getTableData()[0].getRowData()[i].getValue() == column) {
                    columnIndex = i;
                }
            }
        }

        if (!orderby.empty()) {
            columnOrder = orderby[0];
            comparatorOrder = orderby[1];

            columnIndexOrder = 0;
            for (int i = 0; i < headers.size(); i++) {
                if (headers[i] == columnOrder) {
                    columnIndexOrder = i;
                }
            }
        }

        cout << columnOrder << " " << comparatorOrder;

        for (int i = 1; i < table.getTableData().size(); i++) {
            int x = 0;
            vector<string> rowdata;

            for (int j = 0; j < table.getTableData()[i].getRowData().size(); j++) { // Direction concern
                if (table.getTableData()[0].getRowData()[j].getValue() == headers[x]) {
                    if (!where.empty()) {

                        if (operation == ">") {
                            if (table.getTableData()[i].getRowData()[columnIndex].getValue() > comparator) {
                                rowdata.push_back(table.getTableData()[i].getRowData()[j].getValue());
                            }
                        } else if (operation == "<") {
                            if (table.getTableData()[i].getRowData()[columnIndex].getValue() < comparator) {
                                rowdata.push_back(table.getTableData()[i].getRowData()[j].getValue());
                            }
                        } else if (operation == ">=") {
                            if (table.getTableData()[i].getRowData()[columnIndex].getValue() >= comparator) {
                                rowdata.push_back(table.getTableData()[i].getRowData()[j].getValue());
                            }
                        } else if (operation == "<=") {
                            if (table.getTableData()[i].getRowData()[columnIndex].getValue() <= comparator) {
                                rowdata.push_back(table.getTableData()[i].getRowData()[j].getValue());
                            }
                        } else if (operation == "=") {
                            if (table.getTableData()[i].getRowData()[columnIndex].getValue() == comparator) {
                                rowdata.push_back(table.getTableData()[i].getRowData()[j].getValue());
                            }
                        }

                    } else {
                        rowdata.push_back(table.getTableData()[i].getRowData()[j].getValue());
                    }

                    if (x < headers.size() - 1) {
                        x++;
                    }
                }
            }

            if (!rowdata.empty()) {
                dummyrows.push_back(Row(rowdata));
            }
        }

        if (!orderby.empty()) {
            if (comparatorOrder == "descending") {
                sort(dummyrows.begin() + 1, dummyrows.end(), [&columnIndexOrder](Row a, Row b) {return a.getRowData()[columnIndexOrder].getValue() > b.getRowData()[columnIndexOrder].getValue();});
            } else if (comparatorOrder == "ascending"){
                sort(dummyrows.begin() + 1, dummyrows.end(), [&columnIndexOrder](Row a, Row b) {return a.getRowData()[columnIndexOrder].getValue() < b.getRowData()[columnIndexOrder].getValue();});
            }
        }


        retTable.setTableData(dummyrows);
        return retTable;
    }
};

class Update : virtual public Connection {
public:
    void addRow(string tableName, vector<string> row) {
        if (&(this->database->getTable(tableName)) != this->database->getNullTable()) {
            if (row.size() == this->database->getTable(tableName).getTableData()[0].getRowData().size()) {
                if (this->database->getTable(tableName).verifyDataTypes(Row(row))) {
                    this->database->getTable(tableName).getTableData().push_back(Row(row));
                }
            }
        }
    }
};

class Delete : virtual public Connection {
public:
    void deleteRow(string tableName) {
        if (this->database->getTable(tableName).getTableData().size() >= 2) {
            this->database->getTable(tableName).getTableData().pop_back();
        }
    }

    void deleteRow(string tableName, int k) {
        Table& table = this->database->getTable(tableName);
        vector<Row> dummy;

        if (table.getTableData().size() >= 2) {
            for (int i = 0; i < table.getTableData().size(); i++) {
                if (i == k) {
                    continue;
                }

                dummy.push_back(table.getTableData()[i]);
            }

            table.getTableData() = dummy;
        }
    }
};

class Crud: public Create, public Read, public Update, public Delete {
    static Crud* instance;

    Crud() {}

public:
    static Crud* getInstance() {
        if (!instance) instance = new Crud();
        return instance;
    }
};

Crud* Crud::instance = nullptr;

