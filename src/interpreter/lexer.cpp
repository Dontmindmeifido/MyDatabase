#include "lexer.h"

Lexer::Lexer(): dfa_tokenizer(
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

Lexer* Lexer::get_instance() {
    if (!instance) instance = new Lexer();
    return instance;
}

// Returns a list (instructions) of list of tokens that the interpreter can understand
std::vector<std::vector<std::string>> Lexer::run(std::string queries) {
    std::vector<std::vector<std::string>> token_list = {{""}};

    std::vector<int> state = dfa_tokenizer.run(queries);
    for (int i = 0; i < (int)state.size(); i++) {
        int last_index0 = token_list.size() - 1;
        int last_index1 = token_list[last_index0].size() - 1;

        std::vector<std::string>& last_instruction= token_list[last_index0];
        std::string& lastToken = last_instruction[last_index1];

        switch (state[i]) {
            case 0:
                lastToken += queries[i];
                break;
            case 1:
                if (lastToken != "") last_instruction.push_back("");
                break;
            case 2:
                if (lastToken != "") last_instruction.push_back("");
                break;
            case 3:
                token_list.push_back({""});
                break;
            case 4:
                std::cout << "ERROR";
                break;
        }
    }

    // Debug
    std::cout << "\nDEBUG token_lisT\n";
    for (auto x: token_list) {
        for (auto y: x) {
            std::cout << y;
        }
        std::cout << std::endl;
    }
    std::cout << "\nDEBUG token_lisT\n";

    return token_list;
}