#ifndef BUFFER_H
#define BUFFER_H
#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>

template <typename T>
class Buffer {
private:
    std::vector<T> buffer;
    size_t capacity;
    std::mutex mtx;
    std::condition_variable cv;

    size_t count = 0;
    size_t head = 0;
    size_t tail = 0;
public:
    Buffer(size_t capacity) : capacity(capacity) {
        buffer.resize(capacity);
    }

    void produce(T value) {
        std::unique_lock<std::mutex> lock(mtx);
        
        while (count >= capacity) {
            std::cout << "Производитель " << std::this_thread::get_id() << "ждёт: буфер полон" << std::endl;
            std::this_thread::yield();
            cv.wait(lock);
        }

        buffer[tail] = value;
        tail = (tail + 1) % capacity;
        count++;

        std::cout << "Производитель " << std::this_thread::get_id() << " добавил в буфер" << std::endl;

        cv.notify_one();
    }

    T consume() {
        std::unique_lock<std::mutex> lock(mtx);

        while (count == 0) {
            std::cout << "Потребитель " << std::this_thread::get_id() << " ждёт, пока в буфере что-нибудь появится" << std::endl;
            std::this_thread::yield();
            cv.wait(lock);
        }

        T value = buffer[head];
        head = (head + 1) % capacity;
        count--;

        std::cout << "Потребитель " << std::this_thread::get_id() << " извлёк значение " << value << std::endl;

        cv.notify_one();

        return value;
    }

    size_t get_count() {
        return count;
    }
};

#endif