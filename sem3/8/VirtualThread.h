#ifndef VIRTUALTHREAD_H
#define VIRTUALTHREAD_H
#include "Task.h"
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

class VirtualThread {
private:
    std::vector<Task> tasks;

public: 
    void addTask(const Task& task) {
        tasks.push_back(task);
    }

    bool hasTasks() {
        return !tasks.empty();
    }

    void runStep(int threadId) {
        if (hasTasks()) {
            auto it = std::max_element(tasks.begin(), tasks.end(),
            [](Task& a, Task& b) { return a.getPriority() < b.getPriority(); });
        
            Task& task = *it;

            task.increaseCurrentStep();

            std::cout << "Виртуальный поток " << threadId << " выполняет шаг " << task.getCurrentStep() << "/" << task.getSteps() 
            << " задачи " << task.getValue() << " с приоритетом " << task.getPriority() << std::endl;
        
            int stepTime = task.getDuration() / task.getSteps();
            std::this_thread::sleep_for(std::chrono::milliseconds(stepTime));

            if (task.isFinished()) {
                std::cout << "Виртуальный поток " << threadId << " завершил задачу " << task.getValue()
                << ": результат " << task.result() << std::endl;

                tasks.erase(it);
            }
        }
    }
};

#endif