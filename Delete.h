#include "Connection.h"

class Delete : virtual public Connection {
public:
    virtual ~Delete() {}

    void run(std::string tableName, std::string row);
};