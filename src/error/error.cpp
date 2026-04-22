#include "Error.h"

TableDataError::TableDataError() : std::runtime_error("Input does not correspond to table size") {};

CellDataError::CellDataError() : std::runtime_error("Datatype does not correspond to cell type") {};