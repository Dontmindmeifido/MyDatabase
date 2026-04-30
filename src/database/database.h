#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "../error/error.h"
#include "type.h"
#include "meta.h"
#include "validator.h"
#include "../file_manager/observer.h"

class Cell {
    Primitive* cell;

public:
    Cell();
    Cell(const Cell& other);
    Cell(std::string value);
    ~Cell();

    Cell& operator=(const Cell& other);
    Primitive* get_cell() const;
    std::string get_value() const;
    std::string get_type_name() const;
};

class Row {
    std::vector<Cell> cells;

public:
    Row(std::vector<std::string> cells);

    std::vector<Cell>& get_cells();
};

class Table {
    std::string name;
    std::vector<Row> rows;
    Meta meta;

public:
    Table(std::vector<std::string> header, std::vector<std::string> Primitives, std::string name);
    Table(Table* table);


    std::vector<Row>& get_rows();
    void set_rows(std::vector<Row> tableData);
    std::string get_name() const;
    void set_name(std::string name);
    Meta& get_meta(); 
};

class Database: public Observer {
    std::string name;
    std::vector<Table> tables; 
    Validator<Table, Row, Cell> validator;

    static Database* instance;
    Database();

public:
    static Database* get_instance();
    Table* get_table_by_name(std::string name);
    std::vector<Table>& get_tables();
    void create_table(std::string table_name, std::vector<std::string> headers, std::vector<std::string> data_types);
    void delete_table(std::string table_name, std::string row);
    Table* read_table(std::string table_name, std::vector<std::string> headers, std::vector<std::string> where, std::vector<std::string> orderby);
    void insert_row(std::string table_name, std::vector<std::string> row);
    void insert_row(std::string table_name, Row& row);
    void update_cell(std::string table_name, int row, int column, std::string value);
    void join(std::string name, std::vector<std::string> table_names);
};