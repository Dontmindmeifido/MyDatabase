#pragma once
#include "query.h"
#include "lexer.h"
#include "dfa.h"

class Interpreter {
    static Interpreter* instance;
    std::vector<std::string> action_tokens {std::vector<std::string> {"create", "read", "insert", "delete", "join", "update", "load", "store", "undo"}};
    std::vector<std::string> filter_tokens {std::vector<std::string> {"where", "orderby"}};
    std::string destination_token {"in"};
    Dfa dfa_create;
    Dfa dfa_read;
    Dfa dfa_insert;
    Dfa dfa_delete;
    Dfa dfa_join;
    Dfa dfa_update;
    Dfa command;
    Dfa keyword;

    Interpreter();

public:
    static Interpreter* get_instance();
    void run(std::string queries, std::vector<Table*>* READRESPONSE);
    std::vector<std::string> get_all_tokens();
};