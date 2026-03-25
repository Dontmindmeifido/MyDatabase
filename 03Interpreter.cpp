#include "03Interpreter.h"

/*
CREATE (h1:VARCHAR, h2:NUMBER, ...) in TABLE_NAME

ADDROW (c1, c2, ...) in TABLE_NAME

DELROW (k) in TABLE_NAME

DELROW () in TABLE_NAME

READ (h1, h2, ...) in TABLE_NAME where (x,y,z) orderby (ascending)


SYNTAXER:
    ACTION, SOURCE, DESTINATION

*/

void DFA::UpdateState(int transition) {
    currentState = transitions[currentState * alphabet.size() + transition];
}

DFA::DFA(std::vector<int> alphabet, std::vector<int> states, std::vector<int> transitions, std::vector<char> alphabetMap) {
    this->alphabet = alphabet;
    this->states = states;
    this->transitions = transitions;
    this->alphabetMap = alphabetMap;
    this->currentState = 0;
}

std::vector<int> DFA::runDFA(std::string value) {
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

Lexer::Lexer(): 
dfaLexer(std::vector<int>{0, 1, 2}, std::vector<int>{0, 1}, std::vector<int>{0, 0, 1,  0, 0, 1}, std::vector<char> {' ', ';', '%'}) {}

Lexer* Lexer::getInstance() {
    if (!instance) instance = new Lexer();
    return instance;
}

// Returns a list (instructions) of list of tokens that the interpreter can understand
std::vector<std::vector<std::string>> Lexer::runLexer(std::string queries) {
    std::vector<std::vector<std::string>> tokenList;

    removeSpaces(queries);   
    std::vector<std::string> instructions = splitInstructions(queries);
    for (int i = 0; i < instructions.size(); i++) {
        std::vector<int> state = dfaLexer.runDFA(instructions[i]);

        tokenList.push_back({""});
        for (int j = 0; j < instructions[i].length(); j++) {
            int lastInstruction = tokenList.size() - 1;
            int lastToken = tokenList[lastInstruction].size() - 1;

            // Collect char based on state
            if (state[j] == 0 && tokenList[lastInstruction][lastToken] != "") {
                tokenList[lastInstruction].push_back("");
            } else if (state[j] == 1) {
                tokenList[lastInstruction][lastToken] += instructions[i][j];
            }

            // EDGE CASE: last word
            if (j == instructions[i].length() - 1 && tokenList[lastInstruction][lastToken] == "") {
                tokenList[lastInstruction].pop_back();
            }
        }
    }

    // Debug
    std::cout << "\nDEBUG TOKENLIST\n";
    for (auto x: tokenList) {
        for (auto y: x) {
            std::cout << y << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\nDEBUG TOKENLIST\n";

    removeIn(tokenList);

    return tokenList;
}


Interpreter* Interpreter::getInstance() {
    if (!instance) instance = new Interpreter();
    return instance;
}

// Use lexer tokens to compile and run queries
void Interpreter::runInterpreter(std::string queries, std::vector<Table*>* READRESPONSE) {
    Interpreter* Interpreter = Interpreter::getInstance();
    Lexer* lexer = Lexer::getInstance();
    
    // Build queries
    std::vector<std::vector<std::string>> instructions = lexer->runLexer(queries);
    for (std::vector<std::string> token: instructions) {
        Query query;
        
        query.action = lexer->retToLower(token[0]);
        query.destination = token[2];
        query.source = lexer->tokenizeGeneralArgument(token[1]);

        if (lexer->retToLower(query.action) == "read") {
            for (int j = 3; j < token.size(); j += 2) {
                std::string keyword = lexer->retToLower(token[j]);

                if (keyword == "where") {
                    query.where = lexer->tokenizeFilterArgument(token[j + 1]);
                } else if (keyword == "orderby") {
                    query.orderby = lexer->tokenizeFilterArgument(token[j + 1]);
                }
            }
        }

        if (lexer->retToLower(query.action) == "create") {
            std::pair<std::vector<std::string>, std::vector<std::string>> pair = lexer->tokenizeCreateArgument(query.source);
            query.source = pair.first;
            query.types = pair.second;
        }

        // Run query
        Table* queryResponse = query.runQuery();
        if (queryResponse != nullptr) {
            READRESPONSE->push_back(queryResponse);
        }
    }
}

Lexer* Lexer::instance = nullptr;
Interpreter* Interpreter::instance = nullptr;