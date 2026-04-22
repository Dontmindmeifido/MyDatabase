#include "Parser.h"
#include "../Interpreter/Interpreter.h"

std::string Parser::lower(std::string value) {
    transform(value.begin(), value.end(), value.begin(), [](char x) {return tolower(x);});
    return value;
} 

std::string Parser::strip(std::string value) {
    std::string dummy = "";
    for (auto x: value) {
        if (x != ' ' && x != '\n') {
            dummy += x;
        }
    }

    return dummy;
}

std::vector<std::string> Parser::getSpacedWords(std::string queries) {
    std::vector<std::string> wordList = partitionQuery(queries);

    std::string sum = "";
    for (int i = 0; i < (int)wordList.size(); i++) {
        const std::string orgsum = sum;
        for (int j = 0; j < (int)wordList[i].size(); j++) {
            if (wordList[i][j] == '\n') {
                sum += "\n";
            } else {
                sum += " ";
            }
        }

        wordList[i] = orgsum + wordList[i];
    }

    return wordList;
}

std::vector<std::string> Parser::partitionQuery(std::string value) {
    std::vector<std::string> dummy = {""};

    for (auto x: value) {
        if ((x == ' ' || x == '\n') && dummy[dummy.size() - 1] != "") {
            dummy.push_back("");
        }

        dummy[dummy.size() - 1] += x;
    }

    return dummy;
}

std::vector<std::string> Parser::getSnippet(std::string lastWord, Database& db) {
    std::vector<std::string> checks = Interpreter::getInstance()->getAllTokens();

    // Add other tables' data
    for (auto x: db.getTables()) {
        checks.push_back(x.getName());
        for (auto y: x.getRows()[0].getCells()) {
            checks.push_back(y.getValue() + ":" + x.getName());
        }
    }

    std::vector<std::string> ret;

    if (lastWord == "") return ret;
    if (lastWord[0] == '(') lastWord = lastWord.substr(1, -1);

    for (auto word: checks) {
        if ( lower(word.substr(0, std::min(int(word.length()), int(lastWord.length())))) == lower(lastWord.substr(0, std::min(int(word.length()), int(lastWord.length())))) ) {
            ret.push_back(word);
        }
    }

    return ret;
}

std::pair<int, int> Parser::getCursorPosition(std::string queryBuffer) {
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