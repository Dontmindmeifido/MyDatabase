#pragma once
#include "Query.h"
#include "Lexer.h"
#include "Dfa.h"

class Interpreter {
    static Interpreter* instance;
    std::vector<std::string> actionTokens {std::vector<std::string> {"create", "read", "addrow", "delrow"}};
    std::vector<std::string> filterTokens {std::vector<std::string> {"where", "orderby"}};
    std::string destinationToken {"in"};
    Dfa dfaCreate;
    //Dfa dfaRead;
    Dfa dfaUpdate;
    Dfa dfaDelete;


    Interpreter();

public:
    static Interpreter* getInstance();
    void runInterpreter(std::string queries, std::vector<Table*>* READRESPONSE);
    std::vector<std::string> getAllTokens();
};