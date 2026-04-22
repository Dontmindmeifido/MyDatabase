#include "Dfa.h"

Dfa::Dfa(std::vector<int> alphabet, std::vector<int> states, std::vector<int> transitions, std::vector<std::string> alphabetMap) {
    this->alphabet = alphabet;
    this->states = states;
    this->transitions = transitions;
    this->alphabetMap = alphabetMap;
    this->currentState = 0;
}

void Dfa::UpdateState(int transition) {
    currentState = transitions[currentState * alphabet.size() + transition];
}

std::vector<int> Dfa::run(std::string value) {
    std::vector<int> states;

    for (auto chr: value) {
        // Find in alphabetMap
        int transitionIndex = alphabetMap.size() - 1;
        for (int i = 0; i < (int)alphabetMap.size() - 1; i++) {
            if (chr == alphabetMap[i][0]) {
                transitionIndex = i;
                break;
            }
        }

        this->UpdateState(transitionIndex);
        states.push_back(currentState);
    }

    // Reset current state
    this->currentState = 0;

    return states;
}

std::vector<int> Dfa::run(std::vector<std::string> value) {
    std::vector<int> states;

    for (auto chr: value) {
        // Find in alphabetMap
        int transitionIndex = alphabetMap.size() - 1;
        for (int i = 0; i < (int)alphabetMap.size() - 1; i++) {
            if (chr == alphabetMap[i]) {
                transitionIndex = i;
                break;
            }
        }

        this->UpdateState(transitionIndex);
        states.push_back(currentState); // Starts from 1
    }

    // Reset current state
    this->currentState = 0;
    
    return states;
}