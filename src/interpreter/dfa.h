#pragma once
#include <vector>
#include <string>

class Dfa {
    std::vector<int> alphabet;
    std::vector<int> states;
    std::vector<int> transitions;
    std::vector<std::string> alphabetMap;
    int currentState;

public:
    Dfa(std::vector<int> alphabet, std::vector<int> states, std::vector<int> transitions, std::vector<std::string> alphabetMap);

    std::vector<int> run(std::string value);
    std::vector<int> run(std::vector<std::string> value);
    void UpdateState(int transition);
};