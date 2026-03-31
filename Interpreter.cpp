#include "Interpreter.h"

/*
CREATE (h1:VARCHAR, h2:NUMBER, ...) in TABLE_NAME

ADDROW (c1, c2, ...) in TABLE_NAME

DELROW (k) in TABLE_NAME

DELROW () in TABLE_NAME

READ (h1, h2, ...) in TABLE_NAME where (x,y,z) orderby (ascending)


SYNTAXER:
    ACTION, SOURCE, DESTINATION

*/


Interpreter* Interpreter::getInstance() {
    if (!instance) instance = new Interpreter();
    return instance;
}

// Use lexer tokens to compile and run queries
void Interpreter::runInterpreter(std::string queries, std::vector<Table*>* READRESPONSE) {
    Lexer* lexer = Lexer::getInstance();
    
    // Build queries
    std::vector<std::vector<std::string>> instructions = lexer->runLexer(queries);
    for (std::vector<std::string> token: instructions) {
        Query query;
        
        query.action = lexer->lower(token[0]);
        std::cout << query.action;

        // Run query
        // Table* queryResponse = query.runQuery();
        // if (queryResponse != nullptr) {
        //     READRESPONSE->push_back(queryResponse);
        // }
    }
}

Lexer* Lexer::instance = nullptr;
Interpreter* Interpreter::instance = nullptr;