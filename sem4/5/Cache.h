#ifndef CACHE_H
#define CACHE_H
#include <map>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>

template <typename Key, typename Value>
class Cache {
private:
    std::map<Key, Value> data;
    std::mutex mtx;
    std::condition_variable cv;

public:
    inline void set(const Key& key, const Value& value) {
        std::lock_guard<std::mutex> lock(mtx);
        
        data[key] = value;

        std::cout << "Поток " << std::this_thread::get_id() << " добавил {" << key << ", " << value << "}" << std::endl; 
        cv.notify_all();
    }

    inline Value get(const Key& key) {
        std::unique_lock<std::mutex> lock(mtx);

        while (data.find(key) == data.end()) {
            std::cout << "Поток " << std::this_thread::get_id() << " ждёт" << std::endl;
            std::this_thread::yield();
            cv.wait(lock);
        }

        std::cout << "Поток " << std::this_thread::get_id() << " получил data[" << key << "]" << std::endl;
        return data[key];
    }
};

#endif