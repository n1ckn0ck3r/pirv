#include "Group.hpp"
#include <iostream>

Group::Group(const std::string& groupName, const std::vector<Student*>& students) : groupName(groupName), students(students) {}

void Group::addStudent(Student* student) {
    students.push_back(student);
}

void Group::removeStudent(int bookNumber) {
    int initialSize = students.size();
    students.erase(std::remove_if(students.begin(), students.end(), [bookNumber](const Student* student){return student->getBookNumber() == bookNumber;}), students.end());
    int eventualSize = students.size();

    if (initialSize == eventualSize) std::cout << "Никого не удалили, видимо студента с такой зачеткой не существует" << std::endl;
}

double Group::groupMean() const {
    int size = students.size();
    if (size == 0) return 0;

    double sum = 0;
    for (const Student* student : students) sum += student->mean();

    return sum / size;
}