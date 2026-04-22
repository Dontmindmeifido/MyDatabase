#include "Validator.h"

bool Validator::verifyDataTypes(const std::vector<Row>& tableData) const {
    // if (tableDataTypes[0] == "ANY") { // Escape ANY type
    //     return true;
    // }

    for (int i = 1; i < (int)tableData.size(); i++) {
        for (int j = 0; j < (int)tableData[i].getCells().size(); j++) {
            //if (tableDataTypes[j] != tableData[i].getCells()[j].getType())
            return false;
        }
    }

    return true;
}

bool Validator::verifyColumnSize(const std::vector<Row>& tableData) const {
    for (int i = 1; i < (int)tableData.size(); i++) {
        if (tableData[0].getCells().size() != tableData[i].getCells().size()) 
        return false;

    }

    return true;
}