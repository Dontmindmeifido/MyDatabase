#include "parser.h"
#include "../interpreter/interpreter.h"

std::string Parser::get_lower(std::string value) {
    transform(value.begin(), value.end(), value.begin(), [](char x) {return tolower(x);});
    return value;
} 

std::string Parser::get_stripped(std::string value) {
    std::string dummy = "";
    for (auto x: value) {
        if (x != ' ' && x != '\n' && x != ';') {
            dummy += x;
        }
    }

    return dummy;
}

std::vector<std::string> Parser::get_spaced_words(std::string queries) {
    std::vector<std::string> word_list = get_partitioned_query(queries);

    std::string sum = "";
    for (int i = 0; i < (int)word_list.size(); i++) {
        const std::string orgsum = sum;
        for (int j = 0; j < (int)word_list[i].size(); j++) {
            if (word_list[i][j] == '\n') {
                sum += "\n";
            } else {
                sum += " ";
            }
        }

        word_list[i] = orgsum + word_list[i];
    }

    return word_list;
}

std::vector<std::string> Parser::get_partitioned_query(std::string value) {
    std::vector<std::string> dummy = {""};
    
    for (auto x: value) {
        if ((x == ' ' || x == '\n') && dummy[dummy.size() - 1] != "" || dummy[dummy.size() - 1] == ";") {
            dummy.push_back("");
        }

        if (x != ';') {
            dummy[dummy.size() - 1] += x;
        } else {
            dummy.push_back(";");
        }
    }


    return dummy;
}

std::vector<std::string> Parser::get_snippets(std::string lastWord, Database& db) {
    std::vector<std::string> checks = Interpreter::get_instance()->get_all_tokens();

    // Add other tables' data
    for (auto x: db.get_tables()) {
        checks.push_back(x.get_name());
        for (auto y: x.get_rows()[0].get_cells()) {
            checks.push_back(y.get_value() + ":" + x.get_name());
        }
    }

    std::vector<std::string> ret;

    if (lastWord == "") return ret;
    if (lastWord[0] == '(') lastWord = lastWord.substr(1, -1);

    for (auto word: checks) {
        if ( get_lower(word.substr(0, std::min(int(word.length()), int(lastWord.length())))) == get_lower(lastWord.substr(0, std::min(int(word.length()), int(lastWord.length())))) ) {
            ret.push_back(word);
        }
    }

    return ret;
}

std::pair<int, int> Parser::get_cursor_position(std::string queryBuffer) {
    int x = 0;
    int y = 0;

    for (auto chr: queryBuffer) {
        if (chr == '\n') {
            y += 20;
            x = 0;
        } else {
            x += 8;
        }
    }

    return {x, y};
}