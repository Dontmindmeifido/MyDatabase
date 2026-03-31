#include "Delete.h"

void Delete::run(std::string tableName, std::string row) {
    Table* table = this->database->getTable(tableName);

    if (table == nullptr) return;

    if (row.empty()) row = "0";

    if (table->getTableData().size() >= 2) {
        std::vector<Row> dummy;
        for (int i = 0; i < table->getTableData().size(); i++) {
            if (i == stoi(row)) {
                continue;
            }

            dummy.push_back(table->getTableData()[i]);
        }

        table->setTableData(dummy);
    }
}