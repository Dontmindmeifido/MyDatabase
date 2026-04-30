#pragma once
#include <string>
#include <vector>

struct Meta {
    std::vector<std::string> data_types;
    int column_size;

    Meta();
    Meta(std::vector<std::string> data_types);
};