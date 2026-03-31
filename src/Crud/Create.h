#pragma once
#include "Connection.h"

class Create : virtual public Connection {
public:
    virtual ~Create();

    void run(std::string tableName, std::vector<std::string> headers, std::vector<std::string> dataTypes);
};