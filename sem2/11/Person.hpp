#pragma once
#include <string>

class Person {
protected:
    std::string name;
public:
    Person(const std::string& name);
    virtual ~Person() = default;
    virtual void print() const = 0;
    std::string getName() const;
};