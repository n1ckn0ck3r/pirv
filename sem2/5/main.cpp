#include <iostream>
#include <fstream>
#include <vector>

#pragma pack(push, 1)
struct FileHeader {
    char signature[3];
    int version;
    int students;
};
#pragma pack(pop)

void saveFile(const std::vector<std::vector<double>>& studentGrades, const char* name) {
    std::ofstream file(name, std::ios::binary);

    if (!file) {
        std::cout << "Файла нет" << std::endl;
        return;
    }

    FileHeader h;
    h.signature[0] = 'S';
    h.signature[1] = 'H';
    h.signature[2] = 'E';
    h.version = 1;
    h.students = studentGrades.size();
    file.write((char*)&h, sizeof(h));

    int subjects = studentGrades.at(0).size();
    file.write((char*)&subjects, sizeof(subjects));

    for (const auto& student : studentGrades) {
        file.write((char*)student.data(), sizeof(double)*subjects);
    }
    file.close();
}

std::vector<std::vector<double>> loadFile(const char* name) {
    std::ifstream file(name, std::ios::binary);

    if (!file) {
        std::cout << "Файла нет" << std::endl;
        return {};
    }

    FileHeader h;
    file.read((char*)&h, sizeof(h));
    if (h.signature[0] != 'S' || h.signature[1] != 'H' || h.signature[2] != 'E') {
        std::cout << "Сигнатура неверная" << std::endl;
        return {};
    }

    int subjects;
    file.read((char*)&subjects, sizeof(subjects));
    std::vector<std::vector<double>> grades(h.students, std::vector<double>(subjects));

    for (int i = 0; i < h.students; i++) {
        file.read((char*)grades.at(i).data(), sizeof(double)*subjects);
    }

    file.close();
    return grades;
}

int main() {
    std::cout << "Размер структуры: " << sizeof(FileHeader) << " байт" << std::endl; 
    int N, M;
    std::cout << "Введите целое количество студентов > 0: ";
    std::cin >> N;
    while (N <= 0) {
        std::cout << "Число студентов должно быть больше нуля. Введите заново: ";
        std::cin >> N;
    }
    std::cout << "Введите целое количество предметов > 0: ";
    std::cin >> M;
    while (M <= 0) {
        std::cout << "Число предметов должно быть больше нуля. Введите заново: ";
        std::cin >> M;
    }
    std::vector<std::vector<double>> grades;
    for (int i = 0; i < N; i++) {
        std::vector<double> studentGrades;
        for (int j = 0; j < M; j++) {
            int grade;
            std::cout << "Введите оценки студента " << i + 1 << " по предмету " << j + 1 << ": ";
            std::cin >> grade;
            while (grade > 5 || grade < 0) {
                std::cout << "Оценка студента " << i + 1 << " должна быть в пределах от нуля до пяти. Введите заново: ";
                std::cin >> grade;
            }
            studentGrades.push_back(grade);
        }
        grades.push_back(studentGrades);
    }

    saveFile(grades, "grades.bin");
    std::vector<std::vector<double>> loadedGrades = loadFile("grades.bin");

    return 0;
}