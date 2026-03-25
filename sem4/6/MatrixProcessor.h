#ifndef MATRIXPROCESSOR_H
#define MATRIXPROCESSOR_H
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <functional>

template <typename T>
class MatrixProcessor {
private:
    std::vector<std::vector<T>> matrix;
    int rows;
    int cols;
    int thread_count;

    std::mutex mtx;
    std::condition_variable cv;
    int finished_threads = 0;

public:
    MatrixProcessor(const std::vector<std::vector<T>>& matr, int threads) : matrix(matr), thread_count(threads) {
        rows = matrix.size();
        cols = rows > 0 ? matrix[0].size() : 0;
    }

    void apply(std::function<T(T)> func) {
        if (rows == 0) {
            std::cout << "Матрица пуста" << std::endl;
            return;
        }

        finished_threads = 0;
        int total = rows * cols;
        int chunk = total / thread_count;
        int remainder = total & thread_count;

        int start = 0;
        for (int i = 0; i < thread_count; i++) {
            int size = chunk + (i < remainder ? 1 : 0);
            int end = start + size;

            std::thread([this, func, start, end]() {
                std::unique_lock<std::mutex> lock(mtx);
                std::cout << "Поток " << std::this_thread::get_id() << " обрабатывает элементы " << start << "-" << end - 1 << std::endl;
                lock.unlock();

                for (int j = start; j < end; j++) {
                    int row = j / cols;
                    int col = j % cols;
                    matrix[row][col] = func(matrix[row][col]);

                    std::this_thread::yield();
                }

                lock.lock();
                std::cout << "Поток " << std::this_thread::get_id() << " завершил работу" << std::endl;
                
                
                finished_threads++;
                cv.notify_one();
            }).detach();

            start = end;
        }

        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return finished_threads == thread_count; });
    }

    void print() const {
        std::cout << "Матрица: " << std::endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

#endif