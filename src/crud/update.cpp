#include "Update.h"

void Update::run(Database* database, std::string tableName, std::vector<std::string> row) {
    Table* table = database->getTableByName(tableName);

    if (table == nullptr || row.empty()) return;

    std::vector<Row> tableData = table->getRows();
    tableData.push_back(Row(row));

    table->setRows(tableData);
}