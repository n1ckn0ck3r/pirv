#include "Group.hpp"
#include <iostream>
#include <algorithm>

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

void Group::removeStudent(const std::string& name) {
    int initialSize = students.size();
    students.erase(std::remove_if(students.begin(), students.end(), [name](const Student* student){return student->getName() == name;}), students.end());
    int eventualSize = students.size();

    if (initialSize == eventualSize) std::cout << "Никого не удалили, видимо студента с таким именем не существует" << std::endl;
}

void Group::printStudents() const {
    std::cout << "Студенты группы: ";
    for (const Student* student : students) std::cout << " " << student->getName();
    std::cout << std::endl;
}

void Group::sortByMean() {
    std::sort(students.begin(), students.end(), [](const Student* a, const Student* b){ return a->mean() < b->mean(); });
}

void Group::filterByThreshold(double threshold) {
    students.erase(std::remove_if(students.begin(), students.end(), [threshold](const Student* student) { return student->mean() < threshold; }), students.end());
}

double Group::groupMean() const {
    int size = students.size();
    if (size == 0) return 0;

    double sum = 0;
    for (const Student* student : students) sum += student->mean();

    return sum / size;
}

std::string Group::getGroupName() const {
    return groupName;
}

std::vector<Student*> Group::getStudents() const {
    return students;
}