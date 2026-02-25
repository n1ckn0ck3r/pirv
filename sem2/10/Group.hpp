#pragma once
#include "Student.hpp"

class Group {
private:
    std::string groupName;
    std::vector<Student*> students;
public:
    Group(const std::string& groupName, const std::vector<Student*>& students);
    void addStudent(Student* student);
    void removeStudent(int bookNumber);
    double groupMean() const;
};