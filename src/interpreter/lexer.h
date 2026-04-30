#pragma once
#include "../misc/parser.h"
#include "dfa.h"

class Lexer: public Parser {
    static Lexer* instance;
    Dfa dfa_tokenizer;

    Lexer();

public:
    static Lexer* get_instance();

    std::vector<std::vector<std::string>> run(std::string queries);
};