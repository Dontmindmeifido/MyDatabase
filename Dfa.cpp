#include "Dfa.h"

void Dfa::UpdateState(int transition) {
    currentState = transitions[currentState * alphabet.size() + transition];
}

Dfa::Dfa(std::vector<int> alphabet, std::vector<int> states, std::vector<int> transitions, std::vector<char> alphabetMap) {
    this->alphabet = alphabet;
    this->states = states;
    this->transitions = transitions;
    this->alphabetMap = alphabetMap;
    this->currentState = 0;
}

std::vector<int> Dfa::runDFA(std::string value) {
    std::vector<int> states;

    for (auto chr: value) {
        // Find in alphabetMap
        int transitionIndex = alphabetMap.size() - 1;
        for (int i = 0; i < alphabetMap.size() - 1; i++) {
            if (chr == alphabetMap[i]) {
                transitionIndex = i;
                break;
            }
        }

        this->UpdateState(transitionIndex);
        states.push_back(currentState); // Starts from 1
    }

    return states;
}