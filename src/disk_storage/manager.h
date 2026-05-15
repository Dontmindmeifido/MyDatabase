#pragma once
#include <fstream>
#include "../database/database.h"

class Manager {
    static Manager* instance;
    std::string hash_file {".hash.encrypted"};
    std::string crypted_file {"disk/.database.encrypted"};

    Manager() {};
    
public:
    static Manager* get_instance();
    Database* get_database(Database* database, std::string file_name, std::string password);
    void save_database(Database* database, std::string file_name, std::string password);
    void cipher_file(std::string file_in, std::string password);
    void decipher_file(std::string file_out, std::string password);
    void generate_hash(std::string file_name, std::string password);
    bool verify_hash(std::string file_name, std::string password);
};