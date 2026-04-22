#include "Delete.h"

void Delete::run(Database* database, std::string tableName, std::string row) {
    Table* table = database->getTableByName(tableName);

    if (table == nullptr) return;

    if (row.empty()) row = std::to_string(table->getRows().size() - 1);

    if (table->getRows().size() >= 2) {
        std::vector<Row> dummy;
        for (int i = 0; i < (int)table->getRows().size(); i++) {
            if (i == stoi(row)) {
                continue;
            }

            dummy.push_back(table->getRows()[i]);
        }

        table->setRows(dummy);
    }
}