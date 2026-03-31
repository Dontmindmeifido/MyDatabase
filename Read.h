#include "Connection.h"

class Read : virtual public Connection {
public:
    virtual ~Read() {}

    Table* run(std::string tableName, std::vector<std::string> headers, std::vector<std::string> where, std::vector<std::string> orderby);
};