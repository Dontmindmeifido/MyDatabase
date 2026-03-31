#pragma once
#include "../Misc/Parser.h"
#include "Dfa.h"

class Lexer: public Parser {
    static Lexer* instance;
    Dfa dfaTokenizer;

    Lexer();

public:
    static Lexer* getInstance();

    std::vector<std::vector<std::string>> runLexer(std::string queries);
};