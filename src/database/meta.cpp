#include "Meta.h"

Meta::Meta() {}

Meta::Meta(std::vector<std::string> dataTypes) {
    this->dataTypes = dataTypes;
    this->columnSize = dataTypes.size();
}