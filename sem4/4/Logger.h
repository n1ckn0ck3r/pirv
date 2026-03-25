#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

template <typename T>
class Logger {
private:
    std::ofstream log_file;
    std::mutex mtx;
    std::string file_name;

    std::string to_string_impl(const T& value) const {
        if constexpr (std::is_arithmetic_v<T>) {
            return std::to_string(value);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return value;
        }
    }

    inline void write_log(const std::string& entry) {
        if (log_file.is_open()) {
            log_file << entry << std::endl;
            log_file.flush();
        }

        std::cout << entry << std::endl;
    }

public:
    Logger(const std::string& file_name = "log.txt") {
        log_file.open(file_name, std::ios::out | std::ios::app);
    }

    ~Logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void log(const T& message) {
        std::string log_entry = "кто-то оставил сообщение '" + to_string_impl(message) + "'";
        std::lock_guard<std::mutex> lock(mtx);
        write_log(log_entry);
    }
};

#endif