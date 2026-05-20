#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <semaphore>
#include <chrono>

struct Request {
    std::thread::id thread_id;
    bool is_vip;

    bool operator<(const Request& other) const {
        return is_vip < other.is_vip;
    }
};

class ParkingLot {
private:
    int capacity;
    std::counting_semaphore<> semaphore;
    std::mutex mtx;
    std::priority_queue<Request> pq;
    int parked_cars = 0;

public:
    ParkingLot(int initial_capacity) : capacity(initial_capacity), semaphore(initial_capacity) {}

    void park(bool is_vip) {
        auto thread_id = std::this_thread::get_id();

        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Thread " << thread_id << " is trying to park (VIP: " << is_vip << ")" << std::endl;
            pq.push({thread_id, is_vip});
        }

        semaphore.acquire();

        while (true) {
            std::lock_guard<std::mutex> lock(mtx);
            if (!pq.empty() && pq.top().thread_id == thread_id) {
                parked_cars++;
                pq.pop();
                std::cout << "Thread " << thread_id << " parked a car. Total parked: " << parked_cars << std::endl;
                return;
            }
            std::this_thread::yield();
        }
    }

    void leave() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (parked_cars > 0) {
                parked_cars--;
                std::cout << "A car left the parking lot. Total parked: " << parked_cars << std::endl;
            } else {
                std::cout << "No cars to leave." << std::endl;
                return;
            }
        }
    }

    void add_places(int count) {
        std::lock_guard<std::mutex> lock(mtx);
        capacity += count;
        for (int i = 0; i < count; ++i) {
            semaphore.release();
        }
        std::cout << "Added " << count << " parking places. Total capacity: " << capacity << std::endl;
    }
};

void worker(ParkingLot& lot, bool is_vip) {
    try {
        lot.park(is_vip);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        lot.leave();
    } catch (const std::exception& e) {
        std::cout << "Thread " << std::this_thread::get_id() << " encountered an error: " << e.what() << std::endl;
    }
}

int main() {
    ParkingLot lot(2);
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(worker, std::ref(lot), i % 2 == 0);
    }

    for (auto& t : threads) {
        t.detach();
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    lot.add_places(2);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}