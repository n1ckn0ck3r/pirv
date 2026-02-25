#pragma once
#include <fstream>
#include "Student.hpp"

struct StudentInfo {
    std::string name;
    int recordBookNumber;
    int gradeAmount;
    std::vector<double> grades;
};

struct GroupInfo {
    std::string groupName;
    int studentAmount;
    std::vector<StudentInfo> students;
};

class FileManager {
private:
    std::fstream file;
public:
    FileManager(const std::string& fileName);
    ~FileManager();
    void saveFile(const std::string& groupName, const std::vector<Student*>& students);
    GroupInfo loadFile() const;
};