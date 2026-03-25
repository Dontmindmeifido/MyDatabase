#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include "02Query.cpp"
#include "04Parser.cpp"

using namespace std;

/*
CREATE (h1:VARCHAR, h2:NUMBER, ...) in TABLE_NAME

ADDROW (c1, c2, ...) in TABLE_NAME

DELROW (k) in TABLE_NAME

DELROW () in TABLE_NAME

READ (h1, h2, ...) in TABLE_NAME where (x,y,z) orderby (ascending)


SYNTAXER:
    ACTION, SOURCE, DESTINATION

*/

class DFA {
    vector<int> alphabet;
    vector<int> states;
    vector<int> transitions;
    vector<char> alphabetMap; // includes % as last elements - everything else
    int currentState;

    void UpdateState(int transition) {
        currentState = transitions[currentState * alphabet.size() + transition];
    }

public:
    DFA(vector<int> alphabet, vector<int> states, vector<int> transitions, vector<char> alphabetMap) {
        this->alphabet = alphabet;
        this->states = states;
        this->transitions = transitions;
        this->alphabetMap = alphabetMap;
        this->currentState = 0;
    }

    vector<int> runDFA(string value) {
        vector<int> states;

        for (auto chr: value) {
            // Find in alphabetMap
            int transitionIndex = alphabetMap.size() - 1;
            for (int i = 0; i < alphabetMap.size() - 1; i++) {
                if (chr == alphabetMap[i]) {
                    transitionIndex = i;
                    break;
                }
            }

            this->UpdateState(transitionIndex);
            states.push_back(currentState); // Starts from 1
        }

        return states;
    }
};

class Lexer: public Parsing {
    static Lexer* instance;
    DFA dfaTokenizer;

    Lexer(): 
    dfaTokenizer(vector<int>{0, 1, 2}, vector<int>{0, 1}, vector<int>{0, 0, 1,  0, 0, 1}, vector<char> {' ', ';', '%'}) {}

public:
    static Lexer* getInstance() {
        if (!instance) instance = new Lexer();
        return instance;
    }

    vector<vector<string>> runLexer(string queries) {
        vector<vector<string>> wordList;

        removeSpaces(queries);   
        
        vector<string> instructions = splitInstructions(queries);
        for (int k = 0; k < instructions.size(); k++) {
            vector<int> state = dfaTokenizer.runDFA(instructions[k]);

            wordList.push_back({""});
            for (int i = 0; i < instructions[k].length(); i++) {
                int lastInstruction = wordList.size() - 1;
                int lastToken = wordList[lastInstruction].size() - 1;

                // Collect char based on state
                if (state[i] == 0 && wordList[lastInstruction][lastToken] != "") {
                    wordList[lastInstruction].push_back("");
                } else if (state[i] == 1) {
                    wordList[lastInstruction][lastToken] += instructions[k][i];
                }

                // EDGE CASE: last word
                if (i == instructions[k].length() - 1 && wordList[lastInstruction][lastToken] == "") {
                    wordList[lastInstruction].pop_back();
                }
            }
        }

        removeIn(wordList);

        return wordList;
    }

    vector<string> getWords(string queries) {
        vector<string> wordList = partitionQuery(queries);

        string sum = "";
        for (int i = 0; i < wordList.size(); i++) {
            const string orgsum = sum;
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

    vector<string> getSnippet(string lastWord, Database& db) {
        vector<string> checks = {"CREATE", "ADDROW", "DELROW", "READ", "WHERE", "ORDERBY"};
        for (auto x: db.getDatabaseData()) {
            checks.push_back(x.getTableName());
            for (auto y: x.getTableData()[0].getRowData()) {
                checks.push_back(y.getValue() + ":" + x.getTableName());
            }
        }

        vector<string> ret;
        if (lastWord == "") {
            return ret;
        }

        if (lastWord[0] == '(') {
            lastWord = lastWord.substr(1, -1);
        }

        for (auto word: checks) {
            if ( retToLower(word.substr(0, min(int(word.length()), int(lastWord.length())))) == retToLower(lastWord.substr(0, min(int(word.length()), int(lastWord.length())))) ) {
                ret.push_back(word);
            }
        }

        return ret;
    }
};

class Compiler {
    static Compiler* instance;

    Compiler() {}

public:
    static Compiler* getInstance() {
        if (!instance) instance = new Compiler();
        return instance;
    }

    void runCompiler(string queries, Database* database, vector<Table>* READRESPONSE) {
        Lexer* lexer = Lexer::getInstance();
        vector<vector<string>> wordList = lexer->runLexer(queries);
        
        vector<Query> queriesVec;
        Query dummy(database);

        // Build queries
        for (int k = 0; k < wordList.size(); k++) {
            dummy.action = lexer->retToLower(wordList[k][0]);
            dummy.destination = wordList[k][2];
            dummy.source = lexer->splitArgument(wordList[k][1]);

            if (wordList[k].size() >= 4 && lexer->retToLower(dummy.action) == "read") {
                for (int j = 3; j < wordList[k].size(); j += 2) {
                    if (lexer->retToLower(wordList[k][j]) == "where") {
                        dummy.where = lexer->tokenizeArgument(wordList[k][j + 1]);
                    } else if (lexer->retToLower(wordList[k][j]) == "orderby") {
                        dummy.orderby = lexer->tokenizeArgument(wordList[k][j + 1]);
                    }
                }
            }

            queriesVec.push_back(dummy);
        }

        for (auto x: queriesVec) {
            if (x.runQuery().getTableName() != database->getNullTable()->getTableName()) {
                READRESPONSE->push_back(x.runQuery());
            }
        }
    }
};

Lexer* Lexer::instance = nullptr;
Compiler* Compiler::instance = nullptr;