#include "Cache.h"
#include <string>
#include <atomic>
#include <chrono>

int main() {
    Cache<std::string, int> cache;
    std::atomic<int> active_threads(0);

    for (int i = 0; i < 3; i++) {
        active_threads++;
        std::thread([&cache, i, &active_threads]() {
            std::string key = "key" + std::to_string(i);
            int value = i * 100;

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            cache.set(key, value);

            active_threads--;
        }).detach();
    }

    for (int i = 0; i < 3; i++) {
        active_threads++;
        std::thread([&cache, i, &active_threads]() {
            std::string key = "key" + std::to_string(i);

            std::this_thread::sleep_for(std::chrono::milliseconds(70));
            int value = cache.get(key);
            active_threads--;
        }).detach();
    }

    while (active_threads > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    return 0;
}

