#include "Create.h"

Create::~Create() {}

void Create::run(std::string tableName, std::vector<std::string> headers, std::vector<std::string> dataTypes) {
    Table* table = this->database->getTable(tableName);

    if (table != nullptr || headers.empty() || headers.empty()) return;

    std::vector<Table> databaseData = this->database->getDatabaseData();
    databaseData.push_back(Table(headers, dataTypes, tableName));

    this->database->setDatabaseData(databaseData);
}