#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include "CRUDEngine.cpp"

using namespace std;

/*
CREATE (h1:VARCHAR, h2:NUMBER, ...) in TABLE_NAME

ADDROW (c1, c2, ...) in TABLE_NAME

DELROW (k) in TABLE_NAME

DELROW () in TABLE_NAME

READ (h1, h2, ...) in TABLE_NAME


SYNTAXER:
    ACTION, SOURCE, DESTINATION

*/

void toUpper(string& value) {
    transform(value.begin(), value.end(), value.begin(), [](char x) {return toupper(x);});
} 

string retToLower(string value) {
    transform(value.begin(), value.end(), value.begin(), [](char x) {return tolower(x);});
    return value;
} 

void parseString(string& value) {
    string tmp = "";
    bool inside = false;

    for (auto x: value) {
        // Replace newline with space
        if (x == '\n') {
            tmp += " ";
            continue;
        }

        // Check if inside parantheses
        if (x == '(') {
            inside = true;
        } else if (x == ')') {
            inside = false;
        }

        // Remove space inside parantheses
        if (inside) {
            if (x != ' ') {
                tmp += x;
            }
        } else {
            tmp += x;
        }
    }

    value = tmp;
}

void removeIn(vector<string>& values) {
    vector<string> dummy;

    for (auto x: values) {
        if (retToLower(x) != "in") { // Do lowercase
            dummy.push_back(x);
        }
    }

    values = dummy;
}

struct Query {
    string action;
    vector<string> source;
    string destination;

    Table* nullTbl;
    Create* create;
    Read* read;
    Update* update;
    Delete* del;

    Query(Database& db) {
        nullTbl = &(db.getNullTable());
        create = new Create(db);
        read = new Read(db);
        update = new Update(db);
        del = new Delete(db);
    }

    Table runQuery() {
        if (action == "create") { // Syntax HNAME:DATATYPE.
            create->createTable(destination, source);
            return *(this->nullTbl);
        } else if (action == "addrow") {
            update->addRow(destination, source);
            return *(this->nullTbl);
        } else if (action == "delrow") {
            if (source[0] == "") {
                del->deleteRow(destination);
            } else {
                del->deleteRow(destination, stoi(source[0]));
            }

            return *(this->nullTbl);
        } else if (action == "read") {
            return read->readTable(destination, source);
        }
    }
};

struct DFA {
    vector<int> alphabet;
    vector<int> states;
    vector<int> transitions;
    int currentState;

    DFA() {}

    void UpdateState(int transition) {
        currentState = transitions[currentState * alphabet.size() + transition];
    }

};

class Lexer {
    DFA dfa;

public:
    Lexer() {
        dfa.states = vector<int>{0, 1};
        dfa.alphabet = vector<int>{0, 1, 2};
        dfa.transitions = vector<int>{0, 0, 1,  0, 0, 1};
    }

    void runLexer(Database& database, string queries, vector<Table>& READRESPONSE) {
        vector<string> wordList = {""};

        parseString(queries);
        for (int i = 0; i < queries.length(); i++) {
            // State update
            if (queries[i] == ' ') {
                dfa.UpdateState(0);
            } else if (queries[i] == ';') {
                dfa.UpdateState(1);
            } else {
                dfa.UpdateState(2);
            }

            // Collect/Append char based on state
            if (dfa.currentState == 0 && wordList[wordList.size() - 1] != "") {
                wordList.push_back("");
            } else if (dfa.currentState == 1) {
                wordList[wordList.size() - 1] = wordList[wordList.size() - 1] + queries[i];
            }

            // EDGE CASE: last word
            if (i == queries.length() - 1 && wordList[wordList.size() - 1] == "") {
                wordList.pop_back();
            }
        }

        vector<Query> queriesVec;
        Query dummy(database);
        removeIn(wordList);

        // Build queries and verify legality, run
        if (wordList.size() % 3 == 0) {
            for (int i = 0; i < wordList.size(); i++) {
                dummy.action = retToLower(wordList[i]);
                dummy.destination = wordList[i + 2];

                vector<string> dummytemp = {""};
                for (int j = 0; j < wordList[i + 1].length(); j++) {
                    if (wordList[i+1][j] == ',' && dummytemp[dummytemp.size() - 1] != "") {
                        dummytemp.push_back("");
                    } else if (wordList[i+1][j] != '(' && wordList[i+1][j] != ')') {
                        dummytemp[dummytemp.size() - 1] += wordList[i+1][j];
                    }
                }
                dummy.source = dummytemp;

                queriesVec.push_back(dummy);
                i += 2;
            }

            for (auto x: queriesVec) {
                if (x.runQuery().getTableName() != "NULL_TABLE") {
                    READRESPONSE.push_back(x.runQuery());
                }
            }
        } else {
            cout << "SYNTAX ERROR";
        }
    }

    vector<string> getWords(string queries) {
        vector<string> wordList = {""};

        for (auto x: queries) {
            if ((x == ' ' || x == '\n') && wordList[wordList.size() - 1] != "") {
                wordList.push_back("");
            }

            wordList[wordList.size() - 1] += x;
        }

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

        // Ok so basically this is decent, but do calculate the colors here based on what the word is and stuff;
        return wordList;
    }
};