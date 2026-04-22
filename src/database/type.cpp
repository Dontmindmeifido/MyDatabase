#include "Type.h"

bool Primitive::isNumber(std::string value) {
    bool isNumber = true;
    for (auto x: value) {
        if (!(char(x) >= 48 && char(x) <= 57)) {
            isNumber = false;
        }
    }

    return isNumber;
}

bool Primitive::isNumber(char value) {
    bool isNumber = true;
    if (!(char(value) >= 48 && char(value) <= 57)) {
        isNumber = false;
    }

    return isNumber;
}

bool Primitive::isDatetime(std::string value) {
    bool isDatetime = true;
    std::vector<char> FORMAT = {'D', 'D', '-', 'M', 'M', '-', 'Y', 'Y', 'Y', 'Y'};

    for (int i = 0; i < (int)FORMAT.size(); i++) {
        switch(FORMAT[i]) {
            case 'D':
                if (!isNumber(value[i])) isDatetime = false;
                break;
            case 'M':
                    if (!isNumber(value[i])) isDatetime = false;
                break;
            case 'Y':
                if (!isNumber(value[i])) isDatetime = false;
                break;
            case '-':
                if (value[i] != '-') {
                    isDatetime = false;
                }
                break;
        }
    }

    return isDatetime;
}

std::string Primitive::getValue() const {
    return this->value;
}

Varchar::Varchar() {
    this->value = "";
}

Varchar::Varchar(std::string value) {
    this->value = value;
}

Varchar::Varchar(Varchar* obj) {
    this->value = obj->getValue();
}

Varchar* Varchar::clone() const {
    return new Varchar(this->getValue());
}

Number::Number() {
    this->value = "0";
}

Number::Number(std::string value) {
    this->value = value;
}

Number::Number(Number* obj) {
    this->value = obj->getValue();
}

Number* Number::clone() const {
    return new Number(this->getValue());
}

Datetime::Datetime() {
    this->value = "01-01-1970";
}

Datetime::Datetime(std::string value) {
    this->value = value;
}

Datetime::Datetime(Datetime* obj) {
    this->value = obj->getValue();
}

Datetime* Datetime::clone() const {
    return new Datetime(this->getValue());
}