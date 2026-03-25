#include "PriorityQueue.h"
#include <random>
#include <chrono>

int main() {
    PriorityQueue<int> pq;
    std::atomic<int> active_producers(3);
    std::atomic<int> total_processed(0);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-100, 100);
    const int TOTAL_ITEMS = 20;
    
    for (int i = 0; i < 3; i++) {
        std::thread([&]() {
            for (int j = 0; j < 5; j++) {
                int value = dist(gen) + j*j;
                int priority = dist(gen);

                pq.push(value, priority);

                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }

            active_producers--;
        }).detach();
    }

    for (int i = 0; i < 2; i++) {
        std::thread([&]() {
            while (total_processed < TOTAL_ITEMS) {
                if (!pq.empty() || active_producers > 0) {
                    int value = pq.pop();
                } else {
                    if (active_producers == 0 && pq.empty()) {
                        break;
                    }

                    std::this_thread::yield();
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            
        }).detach();
    }

    while (total_processed < TOTAL_ITEMS) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    pq.stop();

    return 0;
}