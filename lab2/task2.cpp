#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <semaphore>
#include <mutex>
#include <chrono>
#include <random>

struct Frame {
    int camera_id;
    int frame_id;
    bool is_important;

    bool operator<(const Frame& other) const {
        if (is_important != other.is_important) {
            return !is_important;
        }

        return frame_id > other.frame_id;
    }
};

std::priority_queue<Frame> pq;
std::mutex mtx;
std::counting_semaphore<> tasks_available(0);

void accelerator_thread(int id) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0, 1);

    while (true) {
        if (dist(gen) > 0.92) {
            std::lock_guard<std::mutex> lock(mtx);
            {
                std::cout << "Ускоритель " << id << " вышел из строя." << std::endl;
                break;
            }
        }

        if (!tasks_available.try_acquire_for(std::chrono::seconds(3))) break;

        Frame task;
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (!pq.empty()) {
                task = pq.top();
                pq.pop();
            } else continue;

            std::cout << "Ускоритель " << id << " взял: камера " << task.camera_id << ", кадр " << task.frame_id << (task.is_important ? " приоритетный." : ".") << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

void camera_thread(int id) {
    for (int i = 1; i <= 5; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        bool important = i % 2 == 0;
        {
            std::lock_guard<std::mutex> lock(mtx);
            pq.push({id, i, important});
        }
        tasks_available.release();
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 1; i <= 3; i++) threads.emplace_back(std::thread(accelerator_thread, i));
    for (int i = 1; i <= 6; i++) threads.emplace_back(std::thread(camera_thread, i));

    for (auto& t : threads) t.join();
    if (pq.empty()) {
        std::cout << "Все кадры обработаны." << std::endl;
    } else {
        std::cout << "Не все кадры удалось обработать, все ускорители вышли из строя." << std::endl;
    }
    return 0;
}