#pragma once
#include <vector>
#include <string>

class Dfa {
    std::vector<int> alphabet;
    std::vector<int> states;
    std::vector<int> transitions;
    std::vector<std::string> alphabet_map;
    int current_state;

public:
    Dfa(std::vector<int> alphabet, std::vector<int> states, std::vector<int> transitions, std::vector<std::string> alphabet_map);
    std::vector<int> run(std::string value);
    std::vector<int> run(std::vector<std::string> value);
    void update_state(int transition);
};