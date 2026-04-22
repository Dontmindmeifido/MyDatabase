#include "Query.h"

Query::Query() {}

Table* Query::runQuery(Database* database) {
    if (action == "create") {
        Create::run(database, table, actionParameters0, actionParameters1);

        return nullptr;
    } else if (action == "addrow") {
        Update::run(database, table, actionParameters0);

        return nullptr;
    } else if (action == "delrow") {
        if (actionParameters0.empty()) {
            Delete::run(database, table, "");
        } else  {
            Delete::run(database, table, actionParameters0[0]);
        }

        return nullptr;
    } else if (action == "read") {
        return Read::run(database, table, actionParameters0, filterWhereParameters, filterOrderByParameters);
    } else {
        return nullptr;
    }
}