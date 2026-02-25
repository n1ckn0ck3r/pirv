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
    void removeStudent(const std::string& name);
    void printStudents() const;
    void sortByMean();
    void filterByThreshold(double threshold);
    double groupMean() const;
    std::string getGroupName() const;
    std::vector<Student*> getStudents() const;
};