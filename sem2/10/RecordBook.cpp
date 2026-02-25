#include "RecordBook.hpp"

RecordBook::RecordBook(int bookNumber) : bookNumber(bookNumber), grades({}) {}
RecordBook::RecordBook(int bookNumber, const std::vector<double>& grades) : bookNumber(bookNumber), grades(grades) {}

double RecordBook::mean() const {
    int N = grades.size();
    if (N == 0) return 0;

    double sum = 0;
    for (auto grade : grades) sum += grade;
    return sum / N;
}

int RecordBook::getBookNumber() const {
    return bookNumber;
}

std::vector<double> RecordBook::getGrades() const {
    return grades;
}

void RecordBook::setGrades(const std::vector<double>& newGrades) {
    grades = newGrades;
} 
