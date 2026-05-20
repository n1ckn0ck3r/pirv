#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <mutex>
#include <semaphore>
#include <chrono>

template <typename T>
class SemaphoreBuffer {
private:
    size_t K;
    std::vector<std::vector<T>> buffers;
    std::vector<std::counting_semaphore<>> empty;
    std::vector<std::counting_semaphore<>> full;
    std::vector<std::mutex> mtx;

public:
    SemaphoreBuffer(size_t k, size_t capaticy) : K(k), buffers(k), mtx(k) {
        for (size_t i = 0; i < K; ++i) {
            empty.emplace_back(std::counting_semaphore<>(capaticy));
            full.emplace_back(std::counting_semaphore<>(0));
        }
    }

    void produce(T value, int buffer_index, int timeout_ms) {
        auto thread_id = std::this_thread::get_id();

        if (empty[buffer_index].try_acquire_for(std::chrono::milliseconds(timeout_ms))) {
            std::lock_guard<std::mutex> lock(mtx[buffer_index]);
            buffers[buffer_index].push_back(value);
            std::cout << "Producer " << thread_id << " produced " << value << " in buffer " << buffer_index << std::endl;
            full[buffer_index].release();
        } else {
            std::cout << "Producer " << thread_id << " timed out while trying to produce in buffer " << buffer_index << std::endl;
        }
    }

    T consume(int buffer_index, int timeous_ms) {
        auto thread_id = std::this_thread::get_id();

        if (full[buffer_index].try_acquire_for(std::chrono::milliseconds(timeous_ms))) {
            std::lock_guard<std::mutex> lock(mtx[buffer_index]);
            T value = buffers[buffer_index].back();
            buffers[buffer_index].pop_back();
            std::cout << "Consumer " << thread_id << " consumed " << value << " from buffer " << buffer_index << std::endl;
            empty[buffer_index].release();
            return value;
        } else {
            std::cout << "Consumer " << thread_id << " timed out while trying to consume from buffer " << buffer_index << std::endl;
            return T{};
        }
    }
};

int get_random_buffer(int K) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, K - 1);
    return dist(gen);
}

void producer(SemaphoreBuffer<int>& sb, int K) {
    for (int i = 0; i < 10; ++i) {
        int buffer_index = get_random_buffer(K);
        sb.produce(i, buffer_index, 100);
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void consumer(SemaphoreBuffer<int>& sb, int K) {
    for (int i = 0; i < 10; ++i) {
        int buffer_index = get_random_buffer(K);
        sb.consume(buffer_index, 100);
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(90));
    }
}

int main() {
    const int K = 3;
    SemaphoreBuffer<int> sb(K, 3);

    std::vector<std::thread> threads;
    for (int i = 0; i < 2; ++i) threads.emplace_back(producer, std::ref(sb), K);
    for (int i = 0; i < 2; ++i) threads.emplace_back(consumer, std::ref(sb), K);

    for (auto& t : threads) t.detach();

    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}