#ifndef PARALLELSUM_H
#define PARALLELSUM_H
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>

template <typename T>
class ParallelSum {
private:
    std::vector<T> data;
    size_t threads;

    T sum = 0;
    std::mutex mtx;
    std::condition_variable cv;

    size_t finishedThreads = 0;

    inline T segment_sum(size_t start, size_t end) {
        T sum = 0;
        for (size_t i = start; i < end; i++) {
            sum += data[i];

            if (i % 21 == 0) {
                std::this_thread::yield();
            }
        }

        return sum;
    }
public:
    ParallelSum(const std::vector<T>& data, size_t threads) : data(data), threads(threads) {}

    T compute_sum() {
        size_t size = data.size();
        size_t segment = size / threads;

        for (size_t i = 0; i < threads; i++) {
            size_t start = i * segment;
            size_t end = (i == threads - 1) ? size : start + segment;

            std::thread([this, start, end]() {
                T segmentSum = segment_sum(start, end);

                {
                    std::lock_guard<std::mutex> lock(mtx);
                    sum += segmentSum;

                    std::cout << "Поток " << std::this_thread::get_id() << ", сумма сегмента = " << segmentSum << std::endl;
                    finishedThreads++;
                }

                cv.notify_one();
            }).detach();
        }

        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this](){ return threads == finishedThreads; });

        return sum;
    }
};

#endif