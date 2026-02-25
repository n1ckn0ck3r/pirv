
#pragma once
#include <iostream>
#include <vector>
#include "Person.hpp"
#include "RecordBook.hpp"

class Student : public Person {
private:
    RecordBook recordBook;
public:
    Student(const std::string& name, int bookNumber): Person(name), recordBook(bookNumber) {}
    Student(const std::string& name, int bookNumber, const std::vector<double>& grades): Person(name), recordBook(bookNumber, grades) {}

    double mean() const {
        return recordBook.mean();
    }

    void addGrade(double grade) {
        std::vector<double> grades = recordBook.getGrades();
        grades.push_back(grade);
        recordBook.setGrades(grades);
    }

    void print() const override {
        std::cout << "У студента по имени " << name << " следующие оценки:";
        for (double grade : recordBook.getGrades()) std::cout << " " << grade;
        std::cout << std::endl;
    }

    std::vector<double> getGrades() const {
        return recordBook.getGrades();
    }

    int getBookNumber() const {
        return recordBook.getBookNumber();
    }
};