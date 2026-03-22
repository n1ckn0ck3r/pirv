#include <iostream>
#include <queue>
#include <mutex>
#include <semaphore>
#include <thread>
#include <chrono>
#include <vector>

std::queue<int> q1, q2;
std::mutex mtx1, mtx2;
std::counting_semaphore<> sem1(0);
std::counting_semaphore<> sem2(0);

void worker(int id) {
    while (true) {
        bool tasked = false;
        int task;
        bool from_q1 = false;

        if (sem1.try_acquire()) {
            std::lock_guard<std::mutex> lock(mtx1);
            task = q1.front();
            q1.pop();
            tasked = true;
            from_q1 = true;
        }

        else if (sem2.try_acquire()) {
            std::lock_guard<std::mutex> lock(mtx2);
            task = q2.front();
            q2.pop();
            tasked = true;
        }

        if (!tasked) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        if (from_q1) {
            std::lock_guard<std::mutex> lock(mtx1);
            std::cout << "Рабочий " << id << " выполняет задачу " << task << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout << "Рабочий " << id << " выполнил задачу " << task << std::endl;
            q1.push(task);
            sem1.release();
        } else {
            std::lock_guard<std::mutex> lock(mtx2);
            std::cout << "Рабочий " << id << " выполняет задачу " << task << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout << "Рабочий " << id << " выполнил задачу " << task << std::endl;
            q2.push(task);
            sem2.release();
        }
    }
}

int main() {
    for (int i = 0; i < 5; i++) {
        q1.push(i);
        sem1.release();
    }

    for (int i = 0; i < 5; i++) {
        q2.push(i);
        sem2.release();
    }

    std::vector<std::thread> threads;

    for (int i = 0; i < 5; i++) {
        threads.emplace_back(std::thread(worker, i));
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
