#include "Lexer.h"

Lexer::Lexer(): dfaTokenizer(
                std::vector<int> {0, 1, 2, 3, 4, 5},     // " ", "\n", ",", ":", ";", "%" 
                std::vector<int> {0, 1, 2, 3, 4},        // "Token", "Space", "Separator", "NewInstruction", "Error"
                std::vector<int> {
                                1, 1, 2, 2, 3, 0,  
                                1, 1, 2, 2, 3, 0,
                                1, 1, 4, 4, 4, 0,
                                1, 1, 4, 4, 4, 0,
                                4, 4, 4, 4, 4, 4
                                }, 
                std::vector<std::string> {" ", "\n", ",", ":", ";", "%"}
                ) {}

Lexer* Lexer::getInstance() {
    if (!instance) instance = new Lexer();
    return instance;
}

// Returns a list (instructions) of list of tokens that the interpreter can understand
std::vector<std::vector<std::string>> Lexer::runLexer(std::string queries) {
    std::vector<std::vector<std::string>> tokenList = {{""}};

    std::vector<int> state = dfaTokenizer.run(queries);
    for (int i = 0; i < (int)state.size(); i++) {
        int lastIndex0 = tokenList.size() - 1;
        int lastIndex1 = tokenList[lastIndex0].size() - 1;

        std::vector<std::string>& lastInstruction= tokenList[lastIndex0];
        std::string& lastToken = lastInstruction[lastIndex1];

        switch (state[i]) {
            case 0:
                lastToken += queries[i];
                break;
            case 1:
                if (lastToken != "") lastInstruction.push_back("");
                break;
            case 2:
                if (lastToken != "") lastInstruction.push_back("");
                break;
            case 3:
                tokenList.push_back({""});
                break;
            case 4:
                std::cout << "ERROR";
                break;
        }
    }

    // Debug
    std::cout << "\nDEBUG TOKENLIST\n";
    for (auto x: tokenList) {
        for (auto y: x) {
            std::cout << y;
        }
        std::cout << std::endl;
    }
    std::cout << "\nDEBUG TOKENLIST\n";

    return tokenList;
}