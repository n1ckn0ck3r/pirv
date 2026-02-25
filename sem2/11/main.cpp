#include "Group.hpp"
#include "FileManager.hpp"
#include <iostream>

int main() {
    Student Kovac("Ковач", 1, {3.1, 4.2, 5.0});
    Student Petrov("Петров", 2, {4.0, 4.5, 4.8});
    Student Osipov("Осипов", 3, {2.5, 3.0, 3.5});
    Student Herbaut("Гербо", 4, {5.0, 5.0, 5.0});
    Student Sokolov("Соколов", 5, {3.0, 3.5, 4.0});
    Student Levashov("Левашов", 6, {4.5, 4.0, 4.2});
    Student Makarenko("Макаренко", 7, {3.5, 4.0, 4.5});
    Student Xu("Сюй", 8, {4.0, 4.5, 5.0});
    Student Polyakov("Поляков", 9, {2.0, 3.0, 3.5});
    Student Johnson("Джонсон", 10, {4.5, 4.5, 4.5});

    Group KUPER("КУПЕР", {&Kovac, &Petrov, &Osipov, &Herbaut, &Sokolov, &Levashov, &Makarenko, &Xu, &Polyakov, &Johnson});

    KUPER.printStudents();
    KUPER.sortByMean();
    std::cout << "После сортировки: " << std::endl;
    KUPER.printStudents();
    KUPER.filterByThreshold(3.6);
    std::cout << "После фильтрации: " << std::endl;
    KUPER.printStudents();
    
    FileManager fileManager("group.dat");
    fileManager.saveFile(KUPER.getGroupName(), KUPER.getStudents());

    GroupInfo loadedGroupInfo = fileManager.loadFile();
    std::cout << "Загруженная группа: " << loadedGroupInfo.groupName << std::endl;
    std::cout << "Число студентов: " << loadedGroupInfo.studentAmount << std::endl;
    std::cout << "Студенты: " << std::endl;
    for (const StudentInfo& studentInfo : loadedGroupInfo.students) {
        std::cout << "Имя: " << studentInfo.name << ", Номер зачетки: " << studentInfo.recordBookNumber << ", Оценки: ";
        for (double grade : studentInfo.grades) {
            std::cout << grade << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}