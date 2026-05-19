#include <iostream>
#include <vector>
#include <chrono>
#include <boost/thread.hpp>
#include <thread>

long fib(int m) {
    if (m < 0) return 0;
    if (m == 1 || m == 0) return 1;

    return fib(m - 1) + fib(m - 2);
}

void threading(int m) {
    long result = fib(m);
}

void boosting(int n, int m) {
    std::vector<boost::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        threads.push_back(boost::thread(threading, m));
    }

    for (int i = 0; i < n; i++) {
        threads[i].join();
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Время выполнения с Boost.Thread на " << n << " потоках: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " мс" << std::endl;
}

void sequenting(int n, int m) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        fib(m);
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Время последовательного выполнения из " << n << " повторений: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " мс" << std::endl;
}

int main() {
    std::cout << "Вычисление 40-го числа Фибоначчи на двух потоках" << std::endl;
    boosting(2, 40);
    sequenting(2, 40);
    std::cout << "--------------" << std::endl;
    std::cout << "Вычисление 40-го числа Фибоначчи на четырех потоках" << std::endl;
    boosting(4, 40);
    sequenting(4, 40);
    std::cout << "--------------" << std::endl;
    std::cout << "Вычисление 40-го числа Фибоначчи на восьми потоках" << std::endl;
    boosting(8, 40);
    sequenting(8, 40);

    return 0;
}