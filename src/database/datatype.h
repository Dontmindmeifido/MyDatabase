#pragma once
#include <string>
#include <vector>
#include <iostream>

class Primitive {
protected:
    std::string value;

public:
    virtual ~Primitive() {};
    static bool is_number(std::string value);
    static bool is_number(char value);
    static bool is_datetime(std::string value);
    std::string get_value() const;
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