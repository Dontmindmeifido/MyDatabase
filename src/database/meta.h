#pragma once
#include <string>
#include <vector>

struct Meta {
    std::vector<std::string> dataTypes;
    int columnSize;

    Meta();
    Meta(std::vector<std::string> dataTypes);
};