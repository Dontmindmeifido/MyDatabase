#include "Connection.h"

class Update : virtual public Connection {
public:
    virtual ~Update() {}

    void run(std::string tableName, std::vector<std::string> row);
};