#pragma once
#include "Query.h"
#include "Lexer.h"

class Interpreter {
    static Interpreter* instance;

    Interpreter() {}

public:
    static Interpreter* getInstance();
    void runInterpreter(std::string queries, std::vector<Table*>* READRESPONSE);
};