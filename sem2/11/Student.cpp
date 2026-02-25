#include "Student.hpp"
#include <iostream>

Student::Student(const std::string& name, int bookNumber) : Person(name), recordBook(bookNumber) {}
Student::Student(const std::string& name, int bookNumber, const std::vector<double>& grades) : Person(name), recordBook(bookNumber, grades) {}

std::vector<double> Student::getGrades() const {
    return recordBook.getGrades();
}

int Student::getBookNumber() const {
    return recordBook.getBookNumber();
}

double Student::mean() const {
    return recordBook.mean();
}

void Student::addGrade(double grade) {
    std::vector<double> grades = getGrades();
    grades.push_back(grade);
    recordBook.setGrades(grades);
}

void Student::print() const {
    std::cout << "У студента по имени " << name << " следующие оценки:";
    for (double grade : getGrades()) std::cout << " " << grade;
    std::cout << std::endl;
}