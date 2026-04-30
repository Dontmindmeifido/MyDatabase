#include "manager.h"

Database* Manager::get_database(std::string file_name, std::string password) {
    Database* database = Database::get_instance();
    database->get_tables().clear();

    std::ifstream database_file(file_name);

    char chr;
    std::string value = "";
    std::string name = "";
    std::vector<std::string> headers;
    std::vector<std::string> data_types;
    std::vector<std::string> dummy_cells;
    while (database_file.get(chr)) {
        if (chr == char(0)) {
            name = value;

            value = "";
        } else if (chr == char(1)) {
            headers.push_back(value);

            value = "";
        } else if (chr == char(2)) {
            data_types.push_back(value);

            value = "";
        } else if (chr == char(3)) {
            database->create_table(name, headers, data_types);

            headers.clear();
            data_types.clear();
            value = "";
        } else if (chr == char(4)) {
            dummy_cells.push_back(value);

            value = "";
        } else if (chr == char(5)) {
            database->insert_row(name, dummy_cells);

            dummy_cells.clear();
            value = "";
        } else {
            value += chr;
        }
    }

    return database;
}

void Manager::save_database(Database* database, std::string file_name, std::string password) {
    std::ofstream database_file(file_name);

    for (Table& table: database->get_tables()) {
        database_file << table.get_name();
        database_file << char(0);

        for (Cell& header: table.get_rows()[0].get_cells()) {
            database_file << header.get_value();
            database_file << char(1);
        }

        for (std::string& data_type: table.get_meta().data_types) {
            database_file << data_type;
            database_file << char(2);
        }

        database_file << char(3);

        for (Row& row: table.get_rows()) {
            for (Cell& cell: row.get_cells()) {
                database_file << cell.get_value() << char(4);
            }
            database_file << char(5);
        }
    }
}

void Manager::cipher_file(std::string file_in, std::string password) {
    std::ifstream in("database.db", std::ios::binary);
    std::ofstream out("database.encrypted", std::ios::binary);

    char chr;
    int key = 0;
    while (in.get(chr)) {
        out.put(chr ^ (password[key++ % password.length()]));
    }
}

Manager* Manager::get_instance() {
    if (Manager::instance != nullptr) return instance;

    instance = new Manager();
    return instance;
}

Manager* Manager::instance = nullptr;
