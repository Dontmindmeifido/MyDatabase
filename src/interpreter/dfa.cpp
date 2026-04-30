#include "dfa.h"

Dfa::Dfa(std::vector<int> alphabet, std::vector<int> states, std::vector<int> transitions, std::vector<std::string> alphabet_map) {
    this->alphabet = alphabet;
    this->states = states;
    this->transitions = transitions;
    this->alphabet_map = alphabet_map;
    this->current_state = 0;
}

void Dfa::update_state(int transition) {
    current_state = transitions[current_state * alphabet.size() + transition];
}

std::vector<int> Dfa::run(std::string value) {
    std::vector<int> states;

    for (auto chr: value) {
        // Find in alphabet_map
        int transitionIndex = alphabet_map.size() - 1;
        for (int i = 0; i < (int)alphabet_map.size() - 1; i++) {
            if (chr == alphabet_map[i][0]) {
                transitionIndex = i;
                break;
            }
        }

        this->update_state(transitionIndex);
        states.push_back(current_state);
    }

    // Reset current state
    this->current_state = 0;

    return states;
}

std::vector<int> Dfa::run(std::vector<std::string> value) {
    std::vector<int> states;

    for (auto chr: value) {
        // Find in alphabet_map
        int transitionIndex = alphabet_map.size() - 1;
        for (int i = 0; i < (int)alphabet_map.size() - 1; i++) {
            if (chr == alphabet_map[i]) {
                transitionIndex = i;
                break;
            }
        }

        this->update_state(transitionIndex);
        states.push_back(current_state); // Starts from 1
    }

    // Reset current state
    this->current_state = 0;
    
    return states;
}