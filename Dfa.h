#include <vector>
#include <string>

class Dfa {
    std::vector<int> alphabet;
    std::vector<int> states;
    std::vector<int> transitions;
    std::vector<char> alphabetMap;
    int currentState;

    void UpdateState(int transition);

public:
    Dfa(std::vector<int> alphabet, std::vector<int> states, std::vector<int> transitions, std::vector<char> alphabetMap);
    std::vector<int> runDFA(std::string value);
};