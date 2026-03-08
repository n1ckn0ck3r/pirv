#ifndef VIRTUALTHREAD_H
#define VIRTUALTHREAD_H
#include <vector>
#include <string>
#include <iostream>
#include <chrono>

class VirtualThread {
private:
    int id;
    std::vector<std::string> tasksQueue;
    int currentIndex;
    bool working;

public: 
    VirtualThread(int id, const std::vector<std::string>& tasksQueue) : id(id), tasksQueue(tasksQueue), currentIndex(0), working(false) {}

    bool hasTasks() {
        return working || currentIndex < tasksQueue.size();
    }

    void increaseCurrentIndex() {
        currentIndex++;
    }

    void runNextTask() {
        if (hasTasks()) {
            if (!working) {  
                std::cout << "Виртуальный поток " << id << " начал " << tasksQueue[currentIndex] << std::endl;
                working = true;
            } else {
                std::cout << "Виртуальный поток " << id << " закончил " << tasksQueue[currentIndex] << std::endl;
                working = false;
                increaseCurrentIndex();
            }
        }
    }

    bool isWorking() {
        return working;
    }
};

#endif