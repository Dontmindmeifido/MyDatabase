#include "Interpreter.h"

/*
CREATE h1:VARCHAR, h2:NUMBER, ... in TABLE_NAME

ADDROW c1, c2, ... in TABLE_NAME

DELROW k in TABLE_NAME

DELROW 2 in TABLE_NAME

READ h1, h2, ... in TABLE_NAME where x y z orderby ascending
*/

Interpreter::Interpreter(): 
             dfaCreate(
                std::vector<int> {0, 1},
                std::vector<int> {0, 1, 2, 3, 4},
                std::vector<int> {
                                  2, 1,
                                  4, 0,
                                  4, 3,
                                  4, 4,
                                  4, 4
                                 },
                std::vector<std::string> {"in", "%"}
             ),
             dfaRead(
                std::vector<int> {0, 1, 2, 3},
                std::vector<int> {0, 1, 2, 3, 4, 5, 6},
                std::vector<int> {
                                  1, 6, 6, 0,
                                  6, 6, 6, 2,
                                  6, 3, 4, 6,
                                  6, 6, 6, 5,
                                  6, 6, 6, 5,
                                  6, 3, 4, 5,
                                  6, 6, 6, 6
                                 },
                std::vector<std::string> {"in", "where", "orderby", "%"}
             ),
             dfaUpdate(
                std::vector<int> {0, 1},
                std::vector<int> {0, 1, 2, 3},
                std::vector<int> {
                                  1, 0,
                                  3, 2,
                                  3, 3,
                                  3, 3
                                 },
                std::vector<std::string> {"in", "%"}
             ), 
             dfaDelete(
                std::vector<int> {0, 1},
                std::vector<int> {0, 1, 2, 3},
                std::vector<int> {
                                  1, 0,
                                  3, 2,
                                  3, 3,
                                  3, 3
                                 },
                std::vector<std::string> {"in", "%"}
             ) {}


Interpreter* Interpreter::getInstance() {
    if (!instance) instance = new Interpreter();
    return instance;
}

void Interpreter::runInterpreter(std::string queries, std::vector<Table*>* READRESPONSE) {
    Lexer* lexer = Lexer::getInstance();
    
    std::vector<std::vector<std::string>> instructions = lexer->runLexer(queries);
    for (std::vector<std::string> token: instructions) {
        Query query;
        
        query.action = lexer->lower(token[0]);
        token.erase(token.begin());
        
        // Find query index
        int index = -1;
        for (int i = 0; i < (int)actionTokens.size(); i++) {
            if (actionTokens[i] == query.action) {
                index = i;
                break;
            }
        }

        std::vector<int> state;
        std::string filter = "";
        switch (index) {
            case -1:
                continue;
            case 0:
                state = dfaCreate.run(token);

                for (int i = 0; i < (int)state.size(); i++) {
                    switch (state[i]) {
                        case 0:
                            query.actionParameters1.push_back(token[i]);
                            break;
                        case 1:
                            query.actionParameters0.push_back(token[i]);
                            break;
                        case 3:
                            query.table = token[i];
                            break;
                        case 4:
                            std::cout << "ERROR CREATE";
                            return;
                    }
                }

                break;
            case 1:
                state = dfaRead.run(token);
                
                for (int i = 0; i < (int)state.size(); i++) {
                    switch (state[i]) {
                        case 0:
                            query.actionParameters0.push_back(token[i]);
                            std::cout << token[i];
                            break;
                        case 2:
                            query.table = token[i];
                            std::cout << token[i];
                            break; 
                        case 3:
                            filter = "where";
                            break;
                        case 4:
                            filter = "orderby";
                            break; 
                        case 5:
                            if (filter == "where") {
                                query.filterWhereParameters.push_back(token[i]);
                                std::cout << token[i];
                            } else if (filter == "orderby") {
                                query.filterOrderByParameters.push_back(token[i]);
                            }
                            break; 
                        case 6:
                            std::cout << "ERROR READ";
                            return;
                    }
                }

                break;
            case 2:
                state = dfaUpdate.run(token);

                for (int i = 0; i < (int)state.size(); i++) {
                    switch (state[i]) {
                        case 0:
                            query.actionParameters0.push_back(token[i]);
                            break;
                        case 2:
                            query.table = token[i];
                            break;
                        case 3:
                            std::cout << "ERROR UPDATE";
                            return;
                    }
                }

                break;
            case 3:
                state = dfaDelete.run(token);

                for (int i = 0; i < (int)state.size(); i++) {
                    switch (state[i]) {
                        case 0:
                            query.actionParameters0.push_back(token[i]);
                            std::cout << std::endl << query.actionParameters0[0] << std::endl;
                            break;
                        case 2:
                            query.table = token[i];
                            std::cout << std::endl << query.table << std::endl;
                            break;
                        case 3:
                            std::cout << "ERROR DELETE";
                            return;
                    }
                }

                break;
        }

        // Run query
        Table* queryResponse = query.runQuery(Database::getInstance());
        if (queryResponse != nullptr) {
            READRESPONSE->push_back(queryResponse);
        }
    }
}

std::vector<std::string> Interpreter::getAllTokens() {
    std::vector<std::string> tokens;

    for (auto token: actionTokens) tokens.push_back(token);
    for (auto token: filterTokens) tokens.push_back(token);

    return tokens;
}

Lexer* Lexer::instance = nullptr;
Interpreter* Interpreter::instance = nullptr;