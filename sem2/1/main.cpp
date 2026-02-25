#include <iostream>

double mean(double* ptr,int N) {
    double sum = 0;
    for (int i = 0; i < N; i++) {
        sum += *ptr;
        ptr++;
    }

    return sum / N;
}

double max(double* ptr, int N) {
    double max = -1;
    for (int i = 0; i < N; i++) {
        if (*ptr > max) max = *ptr;
        ptr++;
    }

    return max;
}

double min(double* ptr, int N) {
    double min = 6;
    for (int i = 0; i < N; i++) {
        if (*ptr < min) min = *ptr;
        ptr++;
    }

    return min;
}

int higherThen(double* ptr, int N, double threshold) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (*ptr > threshold) count++;
        ptr++;
    }

    return count;
}

int main() {
    int N;
    std::cout << "Введите целое число студентов > 0: ";
    std::cin >> N;
    while (N <= 0) {
        std::cout << "Число студентов должно быть больше нуля. Введите заново: ";
        std::cin >> N;
    }

    double* grades = new double[N];

    for (int i = 0; i < N; i++) {
        std::cout << "Введите оценку студента " << i + 1 << ": ";
        std::cin >> grades[i];
        while (grades[i] > 5 || grades[i] < 0) {
            std::cout << "Оценка студента " << i + 1 << " должна быть в пределах от нуля до пяти. Введите заново: ";
            std::cin >> grades[i];
        }
    }

    std::cout << "Средняя оценка студентов: " << mean(grades, N) << std::endl;
    std::cout << "Максимальная оценка студентов: " << max(grades, N) << std::endl;
    std::cout << "Минимальная оценка студентов: " << min(grades, N) << std::endl;
    std::cout << "Число студентов, оценка которых выше 3.5: " << higherThen(grades, N, 3.5) << std::endl;

    delete[] grades;
    grades = nullptr;

    std::cout << grades << std::endl;
    return 0;
}