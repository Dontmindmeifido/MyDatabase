#pragma once
#include <algorithm>
#include "00Database.h"

class Connection {
protected:
    Database* database;

    Connection();

public:
    virtual ~Connection();

    const Database* getConnection();
};

class Create : virtual public Connection {
public:
    virtual ~Create();

    void createTable(std::string tableName, std::vector<std::string> headers, std::vector<std::string> dataTypes);
};

class Read : virtual public Connection {
public:
    virtual ~Read() {}

    Table* readTable(std::string tableName, std::vector<std::string> headers, std::vector<std::string> where, std::vector<std::string> orderby);
};

class Update : virtual public Connection {
public:
    virtual ~Update() {}

    void addRow(std::string tableName, std::vector<std::string> row);
};

class Delete : virtual public Connection {
public:
    virtual ~Delete() {}

    void deleteRow(std::string tableName, std::string row);
};

class Crud: public Create, public Read, public Update, public Delete {
    static Crud* instance;

    Crud();

public:
    static Crud* getInstance();
};
