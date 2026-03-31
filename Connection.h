#pragma once
#include <algorithm>
#include "Database.h"

class Connection {
protected:
    Database* database;

    Connection();

public:
    virtual ~Connection();

    const Database* getConnection();
};