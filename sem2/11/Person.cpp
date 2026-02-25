#include "Person.hpp"

Person::Person(const std::string& name) : name(name) {}

std::string Person::getName() const {
    return name;
}
