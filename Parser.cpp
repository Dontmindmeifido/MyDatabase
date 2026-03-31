#include "Parser.h"

std::string Parser::lower(std::string value) {
    transform(value.begin(), value.end(), value.begin(), [](char x) {return tolower(x);});
    return value;
} 

// Strips all newlines and spaces;
std::string Parser::strip(std::string value) {
    std::string dummy = "";
    for (auto x: value) {
        if (x != ' ' && x != '\n') {
            dummy += x;
        }
    }

    return dummy;
}

// For syntax coloring
std::vector<std::string> Parser::getSpacedWords(std::string queries) {
    std::vector<std::string> wordList = partitionQuery(queries);

    std::string sum = "";
    for (int i = 0; i < wordList.size(); i++) {
        const std::string orgsum = sum;
        for (int j = 0; j < wordList[i].size(); j++) {
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

// For getSpacedWords
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

// For name suggestions
std::vector<std::string> Parser::getSnippet(std::string lastWord, Database& db) {
    std::vector<std::string> checks = {"CREATE", "ADDROW", "DELROW", "READ", "WHERE", "ORDERBY"};
    for (auto x: db.getDatabaseData()) {
        checks.push_back(x.getTableName());
        for (auto y: x.getTableData()[0].getRowData()) {
            checks.push_back(y.getValue() + ":" + x.getTableName());
        }
    }

    std::vector<std::string> ret;
    if (lastWord == "") {
        return ret;
    }

    if (lastWord[0] == '(') {
        lastWord = lastWord.substr(1, -1);
    }

    for (auto word: checks) {
        if ( lower(word.substr(0, std::min(int(word.length()), int(lastWord.length())))) == lower(lastWord.substr(0, std::min(int(word.length()), int(lastWord.length())))) ) {
            ret.push_back(word);
        }
    }

    return ret;
}

// For name suggestions rect positioning
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