#include "Database.h"

bool isNUMBER(std::string value) {
    bool isNumber = true;
    for (auto x: value) {
        if (!(char(x) >= 48 && char(x) <= 57)) {
            isNumber = false;
        }
    }

    return isNumber;
}

bool isNUMBER(char value) {
    bool isNumber = true;
    if (!(char(value) >= 48 && char(value) <= 57)) {
        isNumber = false;
    }

    return isNumber;
}

bool isDATETIME(std::string value) {
    bool isDatetime = true;
    std::vector<char> FORMAT = {'D', 'D', '-', 'M', 'M', '-', 'Y', 'Y', 'Y', 'Y'};

    for (int i = 0; i < FORMAT.size(); i++) {
        switch(FORMAT[i]) {
            case 'D':
                if (!isNUMBER(value[i])) isDatetime = false;
                break;
            case 'M':
                    if (!isNUMBER(value[i])) isDatetime = false;
                break;
            case 'Y':
                if (!isNUMBER(value[i])) isDatetime = false;
                break;
            case '-':
                if (value[i] != '-') {
                    isDatetime = false;
                }
                break;
        }
    }

    return isDatetime;
}


std::string DataType::getValue() const {
    return this->value;
}

std::string DataType::getType() const {
    return this->type;
}


VARCHAR::VARCHAR() {
    this->value = "";
    this->type = "VARCHAR";
}

VARCHAR::VARCHAR(std::string value) {
    this->value = value;
    this->type = "VARCHAR";
}

VARCHAR::VARCHAR(VARCHAR* obj) {
    this->value = obj->getValue();
    this->type = "VARCHAR";
}

VARCHAR* VARCHAR::clone() const {
    return new VARCHAR(this->getValue());
}


NUMBER::NUMBER() {
    this->value = "0";
    this->type = "NUMBER";
}

NUMBER::NUMBER(std::string value) {
    if (isNUMBER(value)) {
        this->value = value;
        this->type = "NUMBER";
    } else {
        std::cout << "ERROR WRONG NUMBER TYPE";
    }
}

NUMBER::NUMBER(NUMBER* obj) {
    this->value = obj->getValue();
    this->type = "NUMBER";
}

NUMBER* NUMBER::clone() const {
    return new NUMBER(this->getValue());
}


DATETIME::DATETIME() {
    this->value = "01-01-1970";
    this->type = "DATETIME";
}

DATETIME::DATETIME(std::string value) {
    if (isDATETIME(value)) {
        this->value = value;
        this->type = "DATETIME";
    } else {
        std::cout << "ERROR WRONG DATETIME TYPE";
    }
}

DATETIME::DATETIME(DATETIME* obj) {
    this->value = obj->getValue();
    this->type = "DATETIME";
}

DATETIME* DATETIME::clone() const {
    return new DATETIME(this->getValue());
}


Cell::Cell() {
    this->data = new VARCHAR();
}

Cell::Cell(const Cell& other) {
    this->data = other.getData()->clone();
}

Cell::Cell(std::string value) {
    if (isDATETIME(value)) {
        this->data = new DATETIME(value);
    } else if (isNUMBER(value)) {
        this->data = new NUMBER(value);
    } else {
        this->data = new VARCHAR(value);
    }
}

Cell::~Cell() {
    delete this->data;
}

Cell& Cell::operator=(const Cell& other) {
    if (&other == this) {
        return *this;
    }

    delete this->data;

    if (other.getDataType() == "VARCHAR") {
        this->data = new VARCHAR(other.getValue());
    } else if (other.getDataType() == "NUMBER") {
        this->data = new NUMBER(other.getValue());
    } else if (other.getDataType() == "DATETIME") {
        this->data = new DATETIME(other.getValue());
    }

    return *this;
}

std::string Cell::getValue() const {
    return this->data->getValue();
}

std::string Cell::getDataType() const {
    return this->data->getType();
}

DataType* Cell::getData() const {
    return this->data;
}


Row::Row(std::vector<std::string> rowData) {
    std::vector<Cell> auxRowData;

    for (auto x: rowData) {
        auxRowData.push_back(Cell(x));
    }

    this->rowData = auxRowData;
}

std::vector<Cell> Row::getRowData() const {
    return this->rowData;
}

void Row::setRowData(std::vector<Cell> rowData) {
    this->rowData = rowData;
}


bool Table::verifyDataTypes(const std::vector<Row>& tableData) {
    if (tableDataTypes[0] == "ANY") { // Escape ANY type
        return true;
    }

    for (int i = 1; i < tableData.size(); i++) {
        for (int j = 0; j < tableData[i].getRowData().size(); j++) {
            if (tableDataTypes[j] != tableData[i].getRowData()[j].getDataType())
            return false;
        }
    }

    return true;
}

bool Table::verifyColumnSize(const std::vector<Row>& tableData) {
    for (int i = 1; i < tableData.size(); i++) {
        if (tableData[0].getRowData().size() != tableData[i].getRowData().size()) 
        return false;

    }

    return true;
}

Table::Table(std::vector<std::string> header, std::vector<std::string> datatypes, std::string tableName) {
    this->tableData.push_back(Row(header));
    this->tableDataTypes = datatypes;
    this->tableName = tableName;
}

std::vector<Row> Table::getTableData() {
    return this->tableData;
}

void Table::setTableData(std::vector<Row> tableData) {
    bool validType = verifyDataTypes(tableData);
    bool validColumns = verifyColumnSize(tableData);

    if (validType && validColumns) {
        this->tableData = tableData;
    } else {
        std::cout << "INVALID WTF";
    }
}

std::string Table::getTableName() {
    return this->tableName;
}


Database::Database() {}

Database* Database::getInstance() {
    if (!instance) instance = new Database();
    return instance;
}

Table* Database::getTable(std::string name) {
    for (int i = 0; i < databaseData.size(); i++) {
        if (databaseData[i].getTableName() == name) {
            return &databaseData[i];
        }
    }

    std::cout << "TABLE NOT FOUND";
    return nullptr;
}

std::vector<Table> Database::getDatabaseData() {
    return this->databaseData;
}

void Database::setDatabaseData(std::vector<Table> databaseData) {
    this->databaseData = databaseData;
}   

Database* Database::instance = nullptr;