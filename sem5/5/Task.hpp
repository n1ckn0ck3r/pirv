#pragma once
#include <chrono>
#include <thread>

class Task {
public:
    int id;
    int required_slots;
    int duration_ms;
    int priority;

    std::chrono::steady_clock::time_point submit_time;

    Task(int id, int slots, int duration, int priority)
        : id(id), required_slots(slots), duration_ms(duration), priority(priority), submit_time(std::chrono::steady_clock::now()) {}

    bool operator<(const Task& other) const {
        return priority < other.priority;
    }

    void execute() {
        std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
    }
};