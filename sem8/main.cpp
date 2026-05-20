#include <iostream>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <random>
#include <algorithm>
#include <execution>
#include <numeric>

struct Telemetry { double value; };

class Dispatcher {
private:
    std::queue<std::vector<Telemetry>> buffer;
    std::mutex mtx;
    std::condition_variable cv;
    bool finished = false;

public:
    void push(std::vector<Telemetry>& data) {
        std::lock_guard<std::mutex> lock(mtx);
        buffer.push(std::move(data));
        cv.notify_one();
    }

    bool pop(std::vector<Telemetry>& data) {
        std::unique_lock lock(mtx);
        cv.wait(lock, [this] { return !buffer.empty() || finished; });
        if (buffer.empty()) return false;
        data = std::move(buffer.front());
        buffer.pop();
        return true;
    }

    void finish() {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
        cv.notify_all();
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    Dispatcher dispatcher;
    std::atomic<double> global_max{0.0};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(10.0, 389.1);
    std::vector<Telemetry> telemetry_data;

    for (int i = 0; i < 1000; ++i) {
        telemetry_data.push_back({dist(gen)});
    }

    std::jthread input_thread([&] {
        dispatcher.push(telemetry_data);
        dispatcher.finish();
    });

    std::vector<std::jthread> pool;
    for (int i = 0; i < 4; ++i) {
        pool.emplace_back([&] {
            std::vector<Telemetry> packet;
            while(dispatcher.pop(packet)) {
                std::sort(std::execution::par, packet.begin(), packet.end(),
                    [](auto& a, auto& b) { return a.value < b.value; });
                
                double sum = std::transform_reduce(std::execution::par_unseq,
                    packet.begin(), packet.end(), 0.0, std::plus<>(),
                    [](auto& t) { return t.value; });

                std::vector<double> scan(packet.size());
                std::transform_inclusive_scan(std::execution::par, packet.begin(), packet.end(),
                    scan.begin(), std::plus<double>(), [](auto& t) { return t.value; });

                double local_max = packet.back().value;
                double expected = global_max.load();
                while (local_max > expected && !global_max.compare_exchange_weak(expected, local_max));

                std::cout << "Packet has been processed. Sum: " << sum << ", Max: " << local_max << std::endl;
            }
        });
    }

    return 0;
}