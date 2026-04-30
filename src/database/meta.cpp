#include "meta.h"

Meta::Meta() {}

Meta::Meta(std::vector<std::string> data_types) {
    this->data_types = data_types;
    this->column_size = data_types.size();
}