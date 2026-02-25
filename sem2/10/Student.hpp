#pragma once
#include "Person.hpp"
#include "RecordBook.hpp"

class Student : public Person {
private:
    RecordBook recordBook;
public:
    Student(const std::string& name, int bookNumber);
    Student(const std::string& name, int bookNumber, const std::vector<double>& grades);

    double mean() const;
    void addGrade(double grade);
    void print() const override;
    std::vector<double> getGrades() const;
    int getBookNumber() const;
};
