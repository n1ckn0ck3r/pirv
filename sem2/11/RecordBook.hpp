#pragma once
#include <vector>

class RecordBook {
private:
    int bookNumber;
    std::vector<double> grades;
public:
    RecordBook(int bookNumber);
    RecordBook(int bookNumber, const std::vector<double>& grades);

    double mean() const;
    int getBookNumber() const;
    std::vector<double> getGrades() const;
    void setGrades(const std::vector<double>& newGrades);
    void printGradeList() const;
};