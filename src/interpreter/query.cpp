#include "query.h"

Table* Create::run(Database* database) {
    database->notify();
    database->create_table(table, action_parameters0, action_parameters1);
    return nullptr;
}

Table* Insert::run(Database* database) {
    database->notify();
    database->insert_row(table, action_parameters0);
    return nullptr;
}

Table* Delete::run(Database* database) {
    database->notify();
    if (action_parameters0.empty()) {
        database->delete_table(table, "");
    } else  {
        database->delete_table(table, action_parameters0[0]);
    }

    return nullptr;
}

Table* Read::run(Database* database) {
    database->notify();
    return database->read_table(table, action_parameters0, filter_where_parameters, filter_order_by_parameters);
}

Table* Update::run(Database* database) {
    database->notify();
    database->update_cell(table, std::stoi(action_parameters0[0]), std::stoi(action_parameters0[1]), action_parameters0[2]);
    return nullptr;
}

Table* Join::run(Database* database) {
    database->notify();
    database->join(table, action_parameters0);
    return nullptr;
}

Table* Load::run(Database* database) {
    database->notify();
    Manager::get_instance()->get_database(action_parameters0[0], "password");
    return nullptr;
}

Table* Store::run(Database* database) {
    Manager::get_instance()->save_database(Database::get_instance(), action_parameters0[0], "password");
    return nullptr;
}

Table* Undo::run(Database* database) {
    Manager::get_instance()->get_database(".version.db", "password");
    return nullptr;
}

Query* QueryFactory::get_query(std::string type) {
    if (type == "create") {
        return new Create();
    } else if (type == "insert") {
        return new Insert();
    } else if (type == "delete") {
        return new Delete();
    } else if (type == "read") {
        return new Read();
    } else if (type == "update") {
        return new Update();
    } else if (type == "join") {
        return new Join();
    } else if (type == "load") {
        return new Load();
    } else if (type == "store") {
        return new Store();
    } else if (type == "undo") {
        return new Undo();
    } else {
        return nullptr;
    }
}

QueryFactory* QueryFactory::instance = nullptr;

QueryFactory* QueryFactory::get_instance() {
    if (instance != nullptr) return instance;

    instance = new QueryFactory();
    return instance;
}