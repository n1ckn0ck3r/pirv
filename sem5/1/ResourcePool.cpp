#include <iostream>
#include <thread>
#include <vector>
#include <semaphore>
#include <mutex>
#include <atomic>
#include <queue>
#include <string>

struct Request {
    std::thread::id thread_id;
    int priority;

    bool operator<(const Request& other) const {
        return priority < other.priority;
    }
};

template <typename T>
class ResourcePool {
private:
    std::vector<T> resources;
    std::counting_semaphore<> semaphore;
    std::mutex mtx;
    std::atomic<int> failed_attempts{0};
    std::priority_queue<Request> pq;

public:
    ResourcePool(std::vector<T> initial_resources) : resources(initial_resources), semaphore(initial_resources.size()) {}

    T acquire(int priority, int timeout_ms) {
        auto thread_id = std::this_thread::get_id();

        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Thread " << thread_id << " is trying to acquire a resource with priority " << priority << std::endl;
            
            pq.push({thread_id, priority});
        }

        auto timeout = std::chrono::milliseconds(timeout_ms);
        if (!semaphore.try_acquire_for(timeout)) {
            std::lock_guard<std::mutex> lock(mtx);
            failed_attempts++;
            std::cout << "Thread " << thread_id << " failed to acquire a resource within timeout" << std::endl;
            throw std::runtime_error("Timeout while acquiring resource");
        }

        while (true) {
            std::lock_guard<std::mutex> lock(mtx);
            if (!pq.empty() && pq.top().thread_id == thread_id) {
                T res = resources.back();
                resources.pop_back();
                pq.pop();

                std::cout << "Thread " << thread_id << " acquired a resource" << std::endl;
                return res;
            }

            std::this_thread::yield();
        }
    }

    void release(T res) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            resources.push_back(res);
            std::cout << "Thread " << std::this_thread::get_id() << " released a resource" << std::endl;    
        }

        semaphore.release();
    }

    void add_resource(T res) {
        std::lock_guard<std::mutex> lock(mtx);
        resources.push_back(res);
        semaphore.release();
        std::cout << "Added new resource: " << res << std::endl;
    }

    int get_failed_attempts() const {
        return failed_attempts.load();
    }
};

void worker(ResourcePool<std::string>& pool, int priority, int timeout_ms) {
    try {
        std::string resource = pool.acquire(priority, timeout_ms);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        pool.release(resource);
    } catch (const std::exception& e) {
        std::cerr << "Worker thread " << std::this_thread::get_id() << " error: " << e.what() << std::endl;
    }
}

int main() {
    ResourcePool<std::string> pool({"Resource1", "Resource2", "Resource3"});

    std::cout << "Initial resources added to the pool." << std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(worker, std::ref(pool), i % 5, 200);
    }

    for (auto& t : threads) {
        t.detach();
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    pool.add_resource("Resource4");

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Total failed attempts: " << pool.get_failed_attempts() << std::endl;
    return 0;
}