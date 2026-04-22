#include "Database.h"

Cell::Cell() {
    this->cell = new Varchar();
}

Cell::Cell(const Cell& other) {
    this->cell = other.getCell()->clone();
}

Cell::Cell(std::string value) {
    if (Primitive::isDatetime(value)) {
        this->cell = new Datetime(value);
        //std::cout << "Value is datetime" << " " << value;
    } else if (Primitive::isNumber(value)) {
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

    if (other.getType() == "Varchar") {
        this->cell = new Varchar(other.getValue());
    } else if (other.getType() == "Number") {
        this->cell = new Number(other.getValue());
    } else if (other.getType() == "Datetime") {
        this->cell = new Datetime(other.getValue());
    }

    return *this;
}

std::string Cell::getValue() const {
    return this->cell->getValue();
}

std::string Cell::getType() const {
    if (dynamic_cast<Number*>(this->cell)) {
        return "Number";
    } else if (dynamic_cast<Datetime*>(this->cell)) {
        return "Datetime";
    } else if (dynamic_cast<Varchar*>(this->cell)) {
        return "Varchar";
    }

    return "No Type";
}

Primitive* Cell::getCell() const {
    return this->cell;
}


Row::Row(std::vector<std::string> rowData) {
    std::vector<Cell> auxRowData;

    for (auto x: rowData) {
        auxRowData.push_back(Cell(x));
    }

    this->cells = auxRowData;
}

std::vector<Cell> Row::getCells() const {
    return this->cells;
}

void Row::setCells(std::vector<Cell> rowData) {
    this->cells = rowData;
}

Table::Table(std::vector<std::string> header, std::vector<std::string> datatypes, std::string tableName) {
    this->rows.push_back(Row(header));
    this->meta = Meta(datatypes);
    this->name = tableName;
}

std::vector<Row> Table::getRows() const {
    return this->rows;
}

void Table::setRows(std::vector<Row> rows) {
    this->rows = rows;
}

std::string Table::getName() const {
    return this->name;
}

Database::Database() {}

Database* Database::getInstance() {
    if (!instance) instance = new Database();
    return instance;
}

Table* Database::getTableByName(std::string name) {
    for (int i = 0; i < (int)this->tables.size(); i++) {
        if (tables[i].getName() == name) {
            return &tables[i];
        }
    }

    std::cout << "TABLE NOT FOUND";
    return nullptr;
}

std::vector<Table> Database::getTables() const {
    return this->tables;
}

void Database::setTables(std::vector<Table> databaseData) {
    this->tables = databaseData;
}   

Database* Database::instance = nullptr;