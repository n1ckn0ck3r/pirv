#pragma once
#include <iostream>
#include <vector>
#include <string>

class Student {
private:
    std::string name;
    std::vector<double> grades;
public:
    explicit Student(const std::string& name): name(name), grades({}) {}; 
    explicit Student(const std::string& name, const std::vector<double>& grades): name(name), grades(grades) {};

    ~Student() {}

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

    void print() const {
        std::cout << "У студента по имени " << name << " следующие оценки:";
        for (double grade : grades) std::cout << " " << grade;
        std::cout << std::endl;
    }

    std::string getName() const {
        return name;
    }

    std::vector<double> getGrades() const {
        return grades;
    }
};