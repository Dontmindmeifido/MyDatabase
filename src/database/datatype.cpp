#include "datatype.h"

bool Primitive::is_number(std::string value) {
    bool is_number = true;
    for (auto x: value) {
        if (!(char(x) >= 48 && char(x) <= 57)) {
            is_number = false;
        }
    }

    return is_number;
}

bool Primitive::is_number(char value) {
    bool is_number = true;
    if (!(char(value) >= 48 && char(value) <= 57)) {
        is_number = false;
    }

    return is_number;
}

bool Primitive::is_datetime(std::string value) {
    bool is_datetime = true;
    std::vector<char> FORMAT = {'D', 'D', '-', 'M', 'M', '-', 'Y', 'Y', 'Y', 'Y'};

    for (int i = 0; i < (int)FORMAT.size(); i++) {
        switch(FORMAT[i]) {
            case 'D':
                if (!is_number(value[i])) is_datetime = false;
                break;
            case 'M':
                    if (!is_number(value[i])) is_datetime = false;
                break;
            case 'Y':
                if (!is_number(value[i])) is_datetime = false;
                break;
            case '-':
                if (value[i] != '-') {
                    is_datetime = false;
                }
                break;
        }
    }

    return is_datetime;
}

std::string Primitive::get_value() const {
    return this->value;
}

Varchar::Varchar() {
    this->value = "";
}

Varchar::Varchar(std::string value) {
    this->value = value;
}

Varchar::Varchar(Varchar* obj) {
    this->value = obj->get_value();
}

Varchar* Varchar::clone() const {
    return new Varchar(this->get_value());
}

Number::Number() {
    this->value = "0";
}

Number::Number(std::string value) {
    this->value = value;
}

Number::Number(Number* obj) {
    this->value = obj->get_value();
}

Number* Number::clone() const {
    return new Number(this->get_value());
}

Datetime::Datetime() {
    this->value = "01-01-1970";
}

Datetime::Datetime(std::string value) {
    this->value = value;
}

Datetime::Datetime(Datetime* obj) {
    this->value = obj->get_value();
}

Datetime* Datetime::clone() const {
    return new Datetime(this->get_value());
}