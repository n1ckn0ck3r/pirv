#pragma once
#include "Person.hpp"

class Teacher : public Person {
private:
    std::string discipline;
public:
    Teacher(const std::string& name, const std::string& discipline);
    void print() const override;
};