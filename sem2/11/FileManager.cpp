#include "FileManager.hpp"
#include <iostream>

FileManager::FileManager(const std::string& fileName) : file(fileName, std::ios::binary | std::ios::in | std::ios::out) {
    if (!file) {
        std::cout << "Ошибка открытия файла" << std::endl;
        return;
    }
}

FileManager::~FileManager() {
    file.close();
}

void FileManager::saveFile(const std::string& groupName, const std::vector<Student*>& students) {
    GroupInfo groupInfo;
    groupInfo.groupName = groupName;
    groupInfo.studentAmount = students.size();

    std::vector<StudentInfo> studentInfos;
    for (const Student* student : students) {
        StudentInfo studentInfo;
        studentInfo.name = student->getName();
        studentInfo.recordBookNumber = student->getBookNumber();
        std::vector<double> grades = student->getGrades();
        studentInfo.gradeAmount = grades.size();
        studentInfo.grades = grades;

        studentInfos.push_back(studentInfo);
    }
    groupInfo.students = studentInfos;

    file.write((char*)&groupInfo, sizeof(groupInfo));
}

GroupInfo FileManager::loadFile() const {
    GroupInfo groupInfo;
    file.read((char*)&groupInfo, sizeof(groupInfo));
    return groupInfo;
}