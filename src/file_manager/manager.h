#pragma once
#include <fstream>
#include "../database/database.h"

class Manager {
    static Manager* instance;

    Manager() {};
public:
    static Manager* get_instance();
    Database* get_database(std::string file_name, std::string password);
    void save_database(Database* database, std::string file_name, std::string password);
    void cipher_file(std::string file_name, std::string password);
};