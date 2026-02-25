#pragma once
#include <vector>
#include "Person.hpp"
#include "Student.hpp"


class Teacher : public Person {
private:
    std::vector<Student*> students;
public: 
    Teacher(const std::string& name) : Person(name), students({}) {}
    Teacher(const std::string& name, const std::vector<Student*>& students) : Person(name), students(students) {}

    void print() const override {
        std::cout << "У учителя " << name << " обучаются такие студенты:";
        for (const Student* student : students) std::cout << " " << student->getName();
        std::cout << std::endl;
    }

    std::vector<Student*> getStudents() const {
        return students;
    }
};