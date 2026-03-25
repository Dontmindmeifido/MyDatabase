#pragma once
#include "02Query.h"
#include "04Parser.h"

class DFA {
    std::vector<int> alphabet;
    std::vector<int> states;
    std::vector<int> transitions;
    std::vector<char> alphabetMap; // includes % as last elements - everything else
    int currentState;

    void UpdateState(int transition);

public:
    DFA(std::vector<int> alphabet, std::vector<int> states, std::vector<int> transitions, std::vector<char> alphabetMap);

    std::vector<int> runDFA(std::string value);
};

// Purpose is with the lexic (regex)
class Lexer: public Parser {
    static Lexer* instance;
    DFA dfaLexer;

    Lexer();

public:
    static Lexer* getInstance();

    // Returns a list (instructions) of list of tokens that the interpreter can understand
    std::vector<std::vector<std::string>> runLexer(std::string queries);
};

// Purpose is to create and run queries
class Interpreter {
    static Interpreter* instance;

    Interpreter() {}

public:
    static Interpreter* getInstance();

    // Use lexer tokens to compile and run queries
    void runInterpreter(std::string queries, std::vector<Table*>* READRESPONSE);
};