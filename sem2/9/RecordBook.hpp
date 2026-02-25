#pragma once
#include <vector>

class RecordBook {
private:   
    int bookNumber;
    std::vector<double> grades;
public:
    RecordBook(int bookNumber) : bookNumber(bookNumber), grades({}) {}
    RecordBook(int bookNumber, const std::vector<double>& grades) : bookNumber(bookNumber), grades(grades) {}


    double mean() const {
        int N = grades.size();
        if (N == 0) return 0;

        double sum = 0;
        for (auto grade : grades) sum += grade;
        return sum / N;
    }

    int getBookNumber() const {
        return bookNumber;
    }

    std::vector<double> getGrades() const {
        return grades;
    }

    void setGrades(const std::vector<double>& newGrades) {
        grades = newGrades;
    } 
};