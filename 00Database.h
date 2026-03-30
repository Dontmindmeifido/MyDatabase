#pragma once
#include <string>
#include <vector>
#include <iostream>

bool isNUMBER(std::string value);

bool isNUMBER(char value);

bool isDATETIME(std::string value);

class DataType {
protected:
    std::string value;
    std::string type;

public:
    virtual ~DataType() {};

    std::string getValue() const;
    std::string getType() const;

    virtual DataType* clone() const = 0;
};

class VARCHAR: public DataType {
public:
    VARCHAR();
    VARCHAR(std::string value);
    VARCHAR(VARCHAR* obj);

    VARCHAR* clone() const override;
};

class NUMBER: public DataType {
public: 
    NUMBER();
    NUMBER(std::string value);
    NUMBER(NUMBER* obj);

    NUMBER* clone() const override;
};

class DATETIME: public DataType {
public:
    DATETIME();
    DATETIME(std::string value);
    DATETIME(DATETIME* obj);

    DATETIME* clone() const override;
};

class Cell {
    DataType* data;

public:
    Cell();
    Cell(const Cell& other);
    Cell(std::string value);
    ~Cell();

    Cell& operator=(const Cell& other);

    std::string getValue() const;
    std::string getDataType() const;
    DataType* getData() const;
};

class Row {
    std::vector<Cell> rowData;

public:
    Row(std::vector<std::string> rowData);
    std::vector<Cell> getRowData() const;

    void setRowData(std::vector<Cell> rowData);
};
// Make a validator class whose sole purpose is to validate data on table creation / rows, etc...
class Table {
    std::vector<Row> tableData;
    std::vector<std::string> tableDataTypes;
    std::string tableName;

    bool verifyDataTypes(const std::vector<Row>& tableData);
    bool verifyColumnSize(const std::vector<Row>& tableData);

public:
    Table(std::vector<std::string> header, std::vector<std::string> datatypes, std::string tableName);

    std::vector<Row> getTableData();
    void setTableData(std::vector<Row> tableData);

    std::string getTableName();
};

class Database {
    static Database* instance;
    std::vector<Table> databaseData; 

    Database();

public:
    static Database* getInstance();

    Table* getTable(std::string name);
    std::vector<Table> getDatabaseData();

    void setDatabaseData(std::vector<Table> databaseData);
};