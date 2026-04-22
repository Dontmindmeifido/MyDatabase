#pragma once
#include <stdexcept>

class TableDataError : public std::runtime_error {
public:
    TableDataError();
};

class CellDataError : public std::runtime_error {
public:
    CellDataError();
};