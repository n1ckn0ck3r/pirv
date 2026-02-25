#include "Teacher.hpp"
#include <iostream>

Teacher::Teacher(const std::string& name, const std::string& discipline) : Person(name), discipline(discipline) {}

void Teacher::print() const {
    std::cout << "Учитель " << name << " ведет дисциплину " << discipline << std::endl;
}