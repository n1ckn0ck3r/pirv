#include <iostream>
#include <vector>

double meanStudent(const std::vector<double>& studentGrades) {
    int M = studentGrades.size();
    double sum = 0;

    for (int i = 0; i < M; i++) {
        sum += studentGrades.at(i);
    }

    return sum / M;
}

double meanSubject(const std::vector<std::vector<double>>& grades, int subNumber) {
    int N = grades.size();
    int M = grades.at(0).size();

    if (subNumber < 0 || subNumber >= M) {
        return -1;
    }

    double sum = 0;
    for (int i = 0; i < N; i++) {
        sum += grades.at(i).at(subNumber);
    }

    return sum / N;
}

int maxMeanStudent(const std::vector<std::vector<double>>& grades) {
    int subNumber = 0;
    int N = grades.size();
    double max = -1;

    for (int i = 0; i < N; i++) {
        double mean = meanStudent(grades.at(i));
        if (mean > max) {
            max = mean;
            subNumber = i;
        }
    }

    return subNumber + 1;
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

    std::cout << "Средний балл студента 1: " << meanStudent(grades.at(0)) << std::endl;
    std::cout << "Средний балл по предмету 1: " << meanSubject(grades, 0) << std::endl;
    std::cout << "Номер студента с максимальным средним баллом: " << maxMeanStudent(grades) << std::endl;

    return 0;
}