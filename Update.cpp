#include "Update.h"

void Update::run(std::string tableName, std::vector<std::string> row) {
    Table* table = this->database->getTable(tableName);

    if (table == nullptr || row.empty()) return;

    std::vector<Row> tableData = table->getTableData();
    tableData.push_back(Row(row));

    table->setTableData(tableData);
}