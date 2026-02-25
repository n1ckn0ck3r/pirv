#include <iostream>
#include <vector>
#include <algorithm>

double meanStudent(const std::vector<double>& studentGrades) {
    int M = studentGrades.size();
    double sum = 0;

    for (int i = 0; i < M; i++) {
        sum += studentGrades.at(i);
    }

    return sum / M;
}

int main() {
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

    std::vector<std::pair<int, double>> studentMeanGrades;
    for (int i = 0; i < N; i++) {
        studentMeanGrades.push_back({i, meanStudent(grades.at(i))});
    }

    std::sort(studentMeanGrades.begin(), studentMeanGrades.end(), [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
        if (a.first == b.first) {
            return a.first < b.first;
        }
        return a.second > b.second;
    });

    for (const auto& pair : studentMeanGrades) {
        std::cout << "Студент " << pair.first << ", средний балл " << pair.second << std::endl;
    }

    return 0;
}