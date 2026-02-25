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

void filterStudents(std::vector<std::pair<int, double>>& studentMeanGrades, double threshold) {
    studentMeanGrades.erase(std::remove_if(studentMeanGrades.begin(), studentMeanGrades.end(), [threshold](const std::pair<int, double>& pair){return pair.second < threshold;}), studentMeanGrades.end());
}

void stats(const std::vector<std::pair<int, double>>& studentMeanGrades) {
    int otl = 0;
    int otch = 0;

    for (const auto& pair : studentMeanGrades) {
        if (pair.second >= 4.5) otl++;
        else if (pair.second < 3.0) otch++;
    }

    std::cout << "Количество отличников: " << otl << std::endl;
    std::cout << "Количество тех, кого надо отчислить: " << otch << std::endl;
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

    stats(studentMeanGrades);
    filterStudents(studentMeanGrades, 3.0);

    return 0;
}