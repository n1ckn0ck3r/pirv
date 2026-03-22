#include <iostream>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <random>
#include <chrono>
#include <atomic>
#include <boost/thread.hpp>

std::mutex mtx;
std::condition_variable cv;
std::atomic<int> blocks_done(0);

long long sum = 0;
int finished_threads = 0;

void matrix_segment_sum(const std::vector<std::vector<int>>& matrix, int start_row, int end_row) {
    long long local_sum = 0;
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            local_sum += matrix[i][j];
        }
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        sum += local_sum;
        finished_threads++;
        blocks_done1.fetch_add(1);
    }

    cv.notify_one();
}

void matrix_segment_sum(const std::vector<std::vector<int>>& matrix, int start_row, int start_column, int N) {
    long long local_sum = 0;
    for (int i = start_row; i < start_row + N; i++) {
        for (int j = start_column; j < start_column + N; j++) {
            local_sum += matrix[i][j];
        }
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        sum += local_sum;
        finished_threads++;
        blocks_done.fetch_add(1);
    }

    cv.notify_one();
}

int main() {
    int N = 1000;
    int num_threads = 4;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 10);

    std::vector<std::vector<int>> matrix(N, std::vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = dist(gen);
        }
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<boost::thread> threads;

    int rows_per_thread = N / num_threads;

    for (int i = 0; i < num_threads; i++) {
        int start_row = i * rows_per_thread;
        int end_row = (i == num_threads - 1) ? N : start_row + rows_per_thread;

        threads.emplace_back(matrix_segment_sum, std::cref(matrix), start_row, end_row);
    }

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [num_threads]{ return finished_threads == num_threads; });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Сумма " << sum << std::endl;
    std::cout << "Затраченное время " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " мс" << std::endl;
    std::cout << "Количество обработанных блоков " << blocks_done.load() << std::endl;
    return 0;
}