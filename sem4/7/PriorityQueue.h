#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <algorithm>
#include <queue>
#include <atomic>

template <typename T>

class PriorityQueue {
private:
    struct QueueItem {
        T value;
        int priority;

        bool operator<(const QueueItem& other) const {
            return priority < other.priority;
        }
    };

    std::priority_queue<QueueItem> pq;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> is_shutdown;

public:
    PriorityQueue() : is_shutdown(false) {}

    ~PriorityQueue() {
        is_shutdown = true;
        cv.notify_all();
    }

    void push(T value, int priority) {
        std::lock_guard<std::mutex> lock(mtx);
        pq.push({value, priority});
        std::cout << "Поток " << std::this_thread::get_id() << " добавил в очередь элемент " << value << " с приоритетом " << priority << std::endl;
        cv.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx);

        while (pq.empty() && !is_shutdown) {
            std::cout << "Поток " << std::this_thread::get_id() << " ждёт, пока в очереди что-нибудь появится" << std::endl;
            std::this_thread::yield();
            cv.wait(lock);
        }

        if (pq.empty()) {
            std::cout << "Поток " << std::this_thread::get_id() << " завершил работу по извлечению" << std::endl;
            return T();
        }

        QueueItem item = pq.top();
        pq.pop();

        std::cout << "Поток " << std::this_thread::get_id() << " извлек элемент со значением " << item.value << " и приоритетом " << item.priority << std::endl;

        return item.value; 
    }

    void stop() {
        is_shutdown = true;
        cv.notify_all();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return pq.empty();
    }
};

#endif