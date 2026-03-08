#include <iostream>
#include <chrono>

long long summing(int N) {
    long long result = 0;

    for (int i = 1; i <= N; i++) {
        result += i;
    }

    return result;
}

int main() {
    int N;
    std::cout << "Введите натуральное число: ";
    std::cin >> N;
    while (N <= 0) {
        std::cout << "Неправильный ввод, пробуйте ещё раз: ";
        std::cin >> N;
    }

    auto start = std::chrono::high_resolution_clock::now();

    long long sum = summing(N);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Результат: " << sum << std::endl;
    std::cout << "Затраченное время: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " мс" << std::endl;
    return 0;
}