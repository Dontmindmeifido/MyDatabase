#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "../Error/Error.h"
#include "Type.h"
#include "Meta.h"

class Cell {
    Primitive* cell;

public:
    Cell();
    Cell(const Cell& other);
    Cell(std::string value);
    ~Cell();

    Cell& operator=(const Cell& other);
    Primitive* getCell() const;
    std::string getType() const;
    std::string getValue() const;
    void setCell();
};

class Row {
    std::vector<Cell> cells;

public:
    Row(std::vector<std::string> cells);

    std::vector<Cell> getCells() const;
    void setCells(std::vector<Cell> cells);
};

class Table {
    std::string name;
    std::vector<Row> rows;
    Meta meta;

    bool verifyPrimitives(const std::vector<Row>& tableData) const; //TODO ADD IN VALIDATOR
    bool verifyColumnSize(const std::vector<Row>& tableData) const;

public:
    Table(std::vector<std::string> header, std::vector<std::string> Primitives, std::string name);

    std::vector<Row> getRows() const;
    void setRows(std::vector<Row> tableData);
    std::string getName() const;
};

class Database {
    std::string name;
    std::vector<Table> tables; 

    static Database* instance;
    Database();

public:
    static Database* getInstance();

    Table* getTableByName(std::string name); // TODO DEPRECATE
    std::vector<Table> getTables() const;
    void setTables(std::vector<Table> databaseData);
};