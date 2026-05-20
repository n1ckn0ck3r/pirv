#pragma once
#include <iostream>
#include <queue>
#include <semaphore>
#include <mutex>
#include <atomic>
#include "Task.hpp"

class TaskScheduler {
private:
    std::priority_queue<Task> task_queue;
    std::counting_semaphore<> semaphore;
    std::mutex queue_mutex;
    std::atomic<int> completed_tasks;

    std::atomic<bool> stop_flag{false};

public:
    TaskScheduler(int total_resources) : semaphore(total_resources), completed_tasks(0) {}

    void submit(Task task) {
        std::lock_guard<std::mutex> lock(queue_mutex);
        task_queue.push(task);
        std::cout << "Submitted task " << task.id << " with priority " << task.priority << " requiring " << task.required_slots << " slots for " << task.duration_ms << "ms" << std::endl;
    }

    inline void execute_task(Task& task) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            std::cout << "Executing task " << task.id << " with priority " << task.priority << " requiring " << task.required_slots << " slots for " << task.duration_ms << "ms" << std::endl;
        }

        task.execute();

        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            std::cout << "Completed task " << task.id << std::endl;
        }
    }

    void worker() {
        while (!stop_flag) {
            Task task(0, 0, 0, 0);
            bool has_task = false;
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                if (!task_queue.empty()) {
                    task = task_queue.top();
                    if (semaphore.try_acquire_for(std::chrono::milliseconds(100))) {
                        task_queue.pop();
                        has_task = true;
                    }
                }
            }

            if (!has_task) {
                std::this_thread::yield();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            for (int i = 0; i < task.required_slots; ++i) {
                semaphore.acquire();
            }

            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                std::cout << "Acquired resources for task " << task.id << std::endl;
            }

            execute_task(task);

            for (int i = 0; i < task.required_slots; ++i) {
                semaphore.release();
            }

            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                std::cout << "Released resources for task " << task.id << std::endl;
            }

            completed_tasks++;
            std::this_thread::yield();
        }
    }

    void stop() {
        stop_flag = true;
    }

    int get_completed_tasks() const {
        return completed_tasks.load();
    }
};