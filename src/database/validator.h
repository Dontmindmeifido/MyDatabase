#pragma once
#include <vector>
#include "Database.h"

class Validator {
public:
    bool verifyDataTypes(const std::vector<Row>& tableData) const;
    bool verifyColumnSize(const std::vector<Row>& tableData) const;
};