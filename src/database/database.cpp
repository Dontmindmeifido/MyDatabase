#include "database.h"

Cell::Cell() {
    this->cell = new Varchar();
}

Cell::Cell(const Cell& other) {
    this->cell = other.get_cell()->clone();
}

Cell::Cell(std::string value) {
    if (Primitive::is_datetime(value)) {
        this->cell = new Datetime(value);
        //std::cout << "Value is datetime" << " " << value;
    } else if (Primitive::is_number(value)) {
        this->cell = new Number(value);
    } else {
        this->cell = new Varchar(value);
    }
}

Cell::~Cell() {
    delete this->cell;
}

Cell& Cell::operator=(const Cell& other) {
    if (&other == this) {
        return *this;
    }

    delete this->cell;

    if (dynamic_cast<Varchar*>(other.get_cell())) {
        this->cell = new Varchar(other.get_value());
    } else if (dynamic_cast<Number*>(other.get_cell())) {
        this->cell = new Number(other.get_value());
    } else if (dynamic_cast<Datetime*>(other.get_cell())) {
        this->cell = new Datetime(other.get_value());
    }

    return *this;
}

std::string Cell::get_value() const {
    return this->cell->get_value();
}

Primitive* Cell::get_cell() const {
    return this->cell;
}

std::string Cell::get_type_name() const {
    if (dynamic_cast<Varchar*>(this->get_cell())) {
        return "VARCHAR";
    } else if (dynamic_cast<Number*>(this->get_cell())) {
        return "NUMBER";
    } else if (dynamic_cast<Datetime*>(this->get_cell())) {
        return "DATETIME";
    }
}


Row::Row(std::vector<std::string> rowData) {
    std::vector<Cell> auxRowData;

    for (auto x: rowData) {
        auxRowData.push_back(Cell(x));
    }

    this->cells = auxRowData;
}

std::vector<Cell>& Row::get_cells() {
    return this->cells;
}

Table::Table(std::vector<std::string> header, std::vector<std::string> datatypes, std::string tableName) {
    this->rows.push_back(Row(header));
    this->meta = Meta(datatypes);
    this->name = tableName;
}

Table::Table(Table* table) {
    this->rows = table->get_rows();
    this->meta = table->get_meta();
    this->name = table->get_name();
}

std::vector<Row>& Table::get_rows() {
    return this->rows;
}

void Table::set_rows(std::vector<Row> rows) {
    this->rows = rows;
}

std::string Table::get_name() const {
    return this->name;
}

void Table::set_name(std::string name) {
    this->name = name;
}

Meta& Table::get_meta() {
    return this->meta;
}

Database::Database() {
    this->subscribe(VersionControl::get_instance());
}

Database* Database::get_instance() {
    if (!instance) instance = new Database();
    return instance;
}

Table* Database::get_table_by_name(std::string name) {
    for (int i = 0; i < (int)this->tables.size(); i++) {
        if (tables[i].get_name() == name) {
            return &tables[i];
        }
    }

    std::cout << "TABLE NOT FOUND";
    return nullptr;
}

std::vector<Table>& Database::get_tables() {
    return this->tables;
} 

Database* Database::instance = nullptr;

void Database::create_table(std::string table_name, std::vector<std::string> headers, std::vector<std::string> data_types) {
    Table* table = this->get_table_by_name(table_name);

    if (table != nullptr || headers.empty()) return;

    this->get_tables().push_back(Table(headers, data_types, table_name));
}

void Database::delete_table(std::string table_name, std::string row) {
    Table* table = this->get_table_by_name(table_name);

    if (table == nullptr) return;

    std::vector<Row>& rows = table->get_rows();
    if (row.empty()) row = std::to_string(rows.size() - 1);

    if (rows.size() >= 2) {
        for (int i = 0; i < (int)rows.size(); i++) {
            if (i == stoi(row)) {
                rows.erase(rows.begin() + i);
            }
        }
    }
}

Table* Database::read_table(std::string table_name, std::vector<std::string> headers, std::vector<std::string> where, std::vector<std::string> orderby) {
    Table* table = this->get_table_by_name(table_name);

    if (table == nullptr || headers.empty() || where.size() % 3 != 0 || orderby.size() % 2 != 0) return nullptr; // miss empty arguments

    // Get columns
    std::vector<Row> dummyrows = {Row(headers)};
    for (int i = 1; i < (int)table->get_rows().size(); i++) {
        int column = 0;
        std::vector<std::string> rowdata;

        for (int j = 0; j < (int)table->get_rows()[i].get_cells().size(); j++) { // Direction sensitive
            if (table->get_rows()[0].get_cells()[j].get_value() == headers[column]) {
                rowdata.push_back(table->get_rows()[i].get_cells()[j].get_value());

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
        for (int i = 0; i < (int)table->get_rows()[0].get_cells().size(); i++) {
            if (table->get_rows()[0].get_cells()[i].get_value() == column) {
                columnIndex = i;
            }
        }

        for (int i = 1; i < (int)dummyrows.size(); i++) {
            if (operation == ">") {
                if (table->get_rows()[i].get_cells()[columnIndex].get_value() > comparator) {
                    rowdata.push_back(dummyrows[i]);
                }
            } else if (operation == "<") {
                if (table->get_rows()[i].get_cells()[columnIndex].get_value() < comparator) {
                    rowdata.push_back(dummyrows[i]);
                }
            } else if (operation == ">=") {
                if (table->get_rows()[i].get_cells()[columnIndex].get_value() >= comparator) {
                    rowdata.push_back(dummyrows[i]);
                }
            } else if (operation == "<=") {
                if (table->get_rows()[i].get_cells()[columnIndex].get_value() <= comparator) {
                    rowdata.push_back(dummyrows[i]);
                }
            } else if (operation == "=") {
                if (table->get_rows()[i].get_cells()[columnIndex].get_value() == comparator) {
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
            sort(dummyrows.begin() + 1, dummyrows.end(), [&columnIndex](Row a, Row b) {return a.get_cells()[columnIndex].get_value() > b.get_cells()[columnIndex].get_value();});
        } else if (order == "ascending"){
            sort(dummyrows.begin() + 1, dummyrows.end(), [&columnIndex](Row a, Row b) {return a.get_cells()[columnIndex].get_value() < b.get_cells()[columnIndex].get_value();});
        }
    }

    Table* retTable = new Table(headers, {"ANY"}, "RESPONSE");
    retTable->set_rows(dummyrows);

    return retTable;
}

void Database::insert_row(std::string table_name, std::vector<std::string> row) {
    Table* table = this->get_table_by_name(table_name);

    if (table == nullptr || row.empty()) return;

    Row new_row = Row(row);
    if (!validator.verify_column_size(table, new_row)) return;
    if (!validator.verify_data_types(table, new_row)) return;
    
    table->get_rows().push_back(new_row);
}

void Database::insert_row(std::string table_name, Row& row) {
    Table* table = this->get_table_by_name(table_name);

    if (!validator.verify_column_size(table, row)) return;
    if (!validator.verify_data_types(table, row)) return;
    
    table->get_rows().push_back(row);
}

void Database::update_cell(std::string table_name, int row, int column, std::string value) {
    Table* table = this->get_table_by_name(table_name);

    std::vector<Row>& rows = table->get_rows();
    if (rows.size() > row) {
        std::vector<Cell>& cells = rows[row].get_cells();

        if (cells.size() > column) {
            Cell& cell = cells[column];
            Cell new_cell = Cell(value);

            if (validator.verify_cell_types(cell, new_cell)) {
                cells[column] = new_cell;
            }
        }
    }
}

void Database::join(std::string name, std::vector<std::string> table_names) {
    if (this->get_table_by_name(name) != nullptr) return;

    Table new_table = Table(this->get_table_by_name(table_names[0]));
    new_table.set_name(name);
    this->tables.push_back(new_table);
    
    for (int i = 1; i < table_names.size(); i++) {
        Table* target_table = this->get_table_by_name(table_names[i]);
        if (target_table == nullptr) return;

        for (Row& row: target_table->get_rows()) {
            this->insert_row(name, row);
        }
    }
}