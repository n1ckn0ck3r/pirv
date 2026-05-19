#include <iostream>
#include <chrono>
#include <atomic>
#include <mutex>
#include <boost/thread.hpp>

int counterWithountSync = 0;
std::atomic<int> atomicCounter(0);
int counterWithMutex = 0;
std::mutex mtx;

void incrWithoutSync() {
    for (int i = 0; i < 1e6; i++) {
        counterWithountSync++;
    }
}

void incrAtomic() {
    for (int i = 0; i < 1e6; i++) {
        atomicCounter++;
    }
}

void incrWithMutex() {
    for (int i = 0; i < 1e6; i++) {
        mtx.lock();
        counterWithMutex++;
        mtx.unlock();
    }
}

int main() {
    std::cout << "--Отсутствие синхронизации--" << std::endl;
    auto start1 = std::chrono::high_resolution_clock::now();

    boost::thread t1(incrWithoutSync);
    boost::thread t2(incrWithoutSync);

    t1.join();
    t2.join();

    auto end1 = std::chrono::high_resolution_clock::now();

    std::cout << "Значение счётчика: " << counterWithountSync << std::endl;  
    std::cout << "Затраченное время: " << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count() << " мс" << std::endl;
    std::cout << "--std::atomic--" << std::endl;
    auto start2 = std::chrono::high_resolution_clock::now();

    boost::thread t3(incrAtomic);
    boost::thread t4(incrAtomic);

    t3.join();
    t4.join();

    auto end2 = std::chrono::high_resolution_clock::now();

    std::cout << "Значение счётчика: " << atomicCounter << std::endl;  
    std::cout << "Затраченное время: " << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count() << " мс" << std::endl;
    std::cout << "--std::mutex--" << std::endl;
    auto start3 = std::chrono::high_resolution_clock::now();

    boost::thread t5(incrWithMutex);
    boost::thread t6(incrWithMutex);

    t5.join();
    t6.join();

    auto end3 = std::chrono::high_resolution_clock::now();

    std::cout << "Значение счётчика: " << counterWithMutex << std::endl;  
    std::cout << "Затраченное время: " << std::chrono::duration_cast<std::chrono::milliseconds>(end3 - start3).count() << " мс" << std::endl;
    return 0;
}