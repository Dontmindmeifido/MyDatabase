#include <string>
#include <cctype>
#include <algorithm>
#include <vector>

using namespace std;

class Parsing {
public:
    void toUpper(string& value) {
        transform(value.begin(), value.end(), value.begin(), [](char x) {return toupper(x);});
    } 

    string retToLower(string value) {
        transform(value.begin(), value.end(), value.begin(), [](char x) {return tolower(x);});
        return value;
    } 

    string retStrip(string value) {
        string dummy = "";
        for (auto x: value) {
            if (x != ' ' && x != '\n') {
                dummy += x;
            }
        }

        return dummy;
    }
    
    void removeSpaces(string& value) {
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

    void removeIn(vector<vector<string>>& values) {
        vector<vector<string>> tempValues;

        for (auto instruction: values) {
            vector<string> dummy;

            for (auto x: instruction) {
                if (retToLower(x) != "in") {
                    dummy.push_back(x);
                }
            }

            tempValues.push_back(dummy);
        }

        values = tempValues;
    }

    vector<string> splitInstructions(string value) {
        vector<string> instructions = {""};
        for (auto x: value) {
            if (x == ';') {
                instructions.push_back("");
            } else {
                instructions[instructions.size() - 1] += x;
            }
        }
        instructions.pop_back();

        return instructions;
    }

    vector<string> splitArgument(string value) {
        vector<string> dummy = {""};

        for (int j = 0; j < value.length(); j++) {
            if (value[j] == ',' && dummy[dummy.size() - 1] != "") {
                dummy.push_back("");
            } else if (value[j] != '(' && value[j] != ')') {
                dummy[dummy.size() - 1] += value[j];
            }
        }

        return dummy;
    }

    vector<string> tokenizeArgument(string value) {
        vector<string> tokens;

        int k = 0;
        for (auto x: value) {
            if (x == ',') {
                k += 1;
                continue;
            }

            if (k == 0 && x != '(' && x != ')') {
                if (tokens.size() == 0) {
                    tokens.push_back("");
                }

                tokens[0] += x;
            } else if (k == 1 && x != '(' && x != ')') {
                if (tokens.size() == 1) {
                    tokens.push_back("");
                }

                tokens[1] += x;
            } else if (k == 2 && x != '(' && x != ')') {
                if (tokens.size() == 2) {
                    tokens.push_back("");
                }
                
                tokens[2] += x;
            }
        }

        return tokens;
    }

    vector<string> partitionQuery(string value) {
        vector<string> dummy = {""};

        for (auto x: value) {
            if ((x == ' ' || x == '\n') && dummy[dummy.size() - 1] != "") {
                dummy.push_back("");
            }

            dummy[dummy.size() - 1] += x;
        }

        return dummy;
    }
};