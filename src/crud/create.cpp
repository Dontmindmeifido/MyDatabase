#include "Create.h"

void Create::run(Database* database, std::string tableName, std::vector<std::string> headers, std::vector<std::string> dataTypes) {
    Table* table = database->getTableByName(tableName);

    if (table != nullptr || headers.empty() || headers.empty()) return;

    std::vector<Table> databaseData = database->getTables();
    databaseData.push_back(Table(headers, dataTypes, tableName));

    database->setTables(databaseData);
}