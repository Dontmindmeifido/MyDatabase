#include "interpreter.h"

/*
CREATE h1:VARCHAR, h2:NUMBER, ... in TABLE_NAME

ADDROW c1, c2, ... in TABLE_NAME

DELROW k in TABLE_NAME

DELROW 2 in TABLE_NAME

READ h1, h2, ... in TABLE_NAME where x y z orderby ascending
*/

Interpreter::Interpreter(): 
             dfa_create(
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
             dfa_read(
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
             dfa_insert(
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
             dfa_update(
                std::vector<int> {0, 1},
                std::vector<int> {0, 1, 2, 3, 4, 5, 6},
                std::vector<int> {
                                  6, 1,
                                  6, 2,
                                  6, 3,
                                  4, 6,
                                  6, 5,
                                  6, 6
                                 },
                std::vector<std::string> {"in", "%"}
             ), 
             dfa_delete(
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
             dfa_join(
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
             command(
                std::vector<int> {0},
                std::vector<int> {0, 1, 2},
                std::vector<int> {
                                  1,
                                  2,
                                  2
                                 },
                std::vector<std::string> {"%"}
            ),
            keyword(
                std::vector<int> {0},
                std::vector<int> {0, 1},
                std::vector<int> {
                                  1,
                                  1
                                 },
                std::vector<std::string> {"%"}
            ) {}


Interpreter* Interpreter::get_instance() {
    if (!instance) instance = new Interpreter();
    return instance;
}

void Interpreter::run(std::string queries, std::vector<Table*>* READRESPONSE) {
    Lexer* lexer = Lexer::get_instance();
    
    std::vector<std::vector<std::string>> instructions = lexer->run(queries);
    for (std::vector<std::string> token: instructions) {
        Query* query = QueryFactory::get_instance()->get_query(lexer->get_lower(token[0]));
        if (query == nullptr) return;

        query->action = lexer->get_lower(token[0]);
        token.erase(token.begin());

        // Find query index
        int index = -1;
        for (int i = 0; i < (int)action_tokens.size(); i++) {
            if (action_tokens[i] == query->action) {
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
                state = dfa_create.run(token);

                for (int i = 0; i < (int)state.size(); i++) {
                    switch (state[i]) {
                        case 0:
                            query->action_parameters1.push_back(token[i]);
                            break;
                        case 1:
                            query->action_parameters0.push_back(token[i]);
                            break;
                        case 3:
                            query->table = token[i];
                            break;
                        case 4:
                            std::cout << "ERROR CREATE";
                            return;
                    }
                }

                break;
            case 1:
                state = dfa_read.run(token);
                
                for (int i = 0; i < (int)state.size(); i++) {
                    switch (state[i]) {
                        case 0:
                            query->action_parameters0.push_back(token[i]);
                            std::cout << token[i];
                            break;
                        case 2:
                            query->table = token[i];
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
                                query->filter_where_parameters.push_back(token[i]);
                                std::cout << token[i];
                            } else if (filter == "orderby") {
                                query->filter_order_by_parameters.push_back(token[i]);
                            }
                            break; 
                        case 6:
                            std::cout << "ERROR READ";
                            return;
                    }
                }

                break;
            case 2:
                state = dfa_insert.run(token);

                for (int i = 0; i < (int)state.size(); i++) {
                    switch (state[i]) {
                        case 0:
                            query->action_parameters0.push_back(token[i]);
                            break;
                        case 2:
                            query->table = token[i];
                            break;
                        case 3:
                            std::cout << "ERROR UPDATE";
                            return;
                    }
                }

                break;
            case 3:
                state = dfa_delete.run(token);

                for (int i = 0; i < (int)state.size(); i++) {
                    switch (state[i]) {
                        case 0:
                            query->action_parameters0.push_back(token[i]);
                            std::cout << std::endl << query->action_parameters0[0] << std::endl;
                            break;
                        case 2:
                            query->table = token[i];
                            std::cout << std::endl << query->table << std::endl;
                            break;
                        case 3:
                            std::cout << "ERROR DELETE";
                            return;
                    }
                }

                break;
            case 4:
                state = dfa_join.run(token);

                for (int i = 0; i < (int)state.size(); i++) {
                    switch (state[i]) {
                        case 0:
                            query->action_parameters0.push_back(token[i]);
                            break;
                        case 2:
                            query->table = token[i];
                            break;
                        case 3:
                            std::cout << "ERROR JOIN";
                            return;
                    }
                }

                break;
            case 5:
                state = dfa_update.run(token);

                for (int i = 0; i < (int)state.size(); i++) {
                    switch (state[i]) {
                        case 1:
                            query->action_parameters0.push_back(token[i]);
                            std::cout << std::endl << query->action_parameters0[0] << std::endl;
                            break;
                        case 2:
                            query->action_parameters0.push_back(token[i]);
                            std::cout << std::endl << query->action_parameters0[0] << std::endl;
                            break;
                        case 3:
                            query->action_parameters0.push_back(token[i]);
                            std::cout << std::endl << query->action_parameters0[0] << std::endl;
                            break;
                        case 5:
                            query->table = token[i];
                            std::cout << std::endl << query->table << std::endl;
                            break;
                        case 6:
                            std::cout << "ERROR DELETE";
                            return;
                    }
                }

                break;
            case 6:
                state = command.run(token);

                for (int i = 0; i < (int)state.size(); i++) {
                    switch (state[i]) {
                        case 1:
                            query->action_parameters0.push_back(token[i]);
                            std::cout << std::endl << query->action_parameters0[0] << std::endl;
                            break;
                        case 2:
                            std::cout << "ERROR COMMAND";
                            return;
                    }
                }

                break;
            case 7:
                state = command.run(token);

                for (int i = 0; i < (int)state.size(); i++) {
                    switch (state[i]) {
                        case 1:
                            query->action_parameters0.push_back(token[i]);
                            std::cout << std::endl << query->action_parameters0[0] << std::endl;
                            break;
                        case 2:
                            std::cout << "ERROR COMMAND";
                            return;
                    }
                }

                break;
            case 8:
                state = keyword.run(token);
                break;
        }

        // Run queries
        Table* queryResponse = query->run(Database::get_instance());
        if (queryResponse != nullptr) {
            READRESPONSE->push_back(queryResponse);
        }

        delete query;
    }
}

std::vector<std::string> Interpreter::get_all_tokens() {
    std::vector<std::string> tokens;

    for (auto token: action_tokens) tokens.push_back(token);
    for (auto token: filter_tokens) tokens.push_back(token);

    return tokens;
}

Lexer* Lexer::instance = nullptr;
Interpreter* Interpreter::instance = nullptr;