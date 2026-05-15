#pragma once
#include <vector>
#include <string>
#include "../database/database.h"
#include "../disk_storage/manager.h"

struct Query {
    std::string action;
    std::vector<std::string> action_parameters0;
    std::vector<std::string> action_parameters1;
    std::string table;
    std::vector<std::string> filter_where_parameters;
    std::vector<std::string> filter_order_by_parameters;
    std::string database_file {".database.version_control"};
    std::string undo_key {"undo_key"};

    virtual Table* run(Database* database) = 0;
    virtual ~Query() = default;
};

struct Create: public Query {
    Table* run(Database* database) override;
};

struct Read: public Query {
    Table* run(Database* database) override;
};

struct Insert: public Query {
    Table* run(Database* database) override;
};

struct Delete: public Query {
    Table* run(Database* database) override;
};

struct Update: public Query {
    Table* run(Database* database) override;
};

struct Join: public Query {
    Table* run(Database* database) override;
};

struct Load: public Query {
    Table* run(Database* database) override;
};

struct Store: public Query {
    Table* run(Database* database) override;
};

struct Undo: public Query {
    Table* run(Database* database) override;
};

class QueryFactory {
    static QueryFactory* instance;

    QueryFactory() = default;

public:
    static QueryFactory* get_instance();
    Query* get_query(std::string type);
};