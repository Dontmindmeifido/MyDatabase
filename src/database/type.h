#pragma once
#include <string>
#include <vector>
#include <iostream>

class Primitive {
protected:
    std::string value;

public:
    virtual ~Primitive() {};

    static bool isNumber(std::string value);
    static bool isNumber(char value);
    static bool isDatetime(std::string value);
    std::string getValue() const;
    virtual Primitive* clone() const = 0;
};

class Varchar: public Primitive {
public:
    Varchar();
    Varchar(std::string value);
    Varchar(Varchar* obj);

    Varchar* clone() const override;
};

class Number: public Primitive {
public: 
    Number();
    Number(std::string value);
    Number(Number* obj);

    Number* clone() const override;
};

class Datetime: public Primitive {
public:
    Datetime();
    Datetime(std::string value);
    Datetime(Datetime* obj);

    Datetime* clone() const override;
};