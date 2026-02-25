
#pragma once
#include <iostream>
#include <vector>
#include <Person.hpp>

class Student : public Person {
private:
    std::vector<double> grades;
public:
    Student(const std::string& name): Person(name), grades({}) {}
    Student(const std::string& name, const std::vector<double>& grades): Person(name), grades(grades) {}

    double mean() const {
        double sum = 0;
        int N = grades.size();

        if (N == 0) return 0;
        for (double grade : grades) sum += grade;

        return sum / N;
    }

    void addGrade(double grade) {
        grades.push_back(grade);
    }

    void print() const override {
        std::cout << "У студента по имени " << name << " следующие оценки:";
        for (double grade : grades) std::cout << " " << grade;
        std::cout << std::endl;
    }

    std::vector<double> getGrades() const {
        return grades;
    }
};