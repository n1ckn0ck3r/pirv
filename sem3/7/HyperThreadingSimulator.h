#ifndef HYPERTHREADINGSIMULATOR_H
#define HYPERTHREADINGSIMULATOR_H
#include "VirtualThread.h"

class HyperThreadingSimulator {
private:
    VirtualThread vt1;
    VirtualThread vt2;
public:
    HyperThreadingSimulator(const std::vector<std::string>& tasksQueue) : vt1(1, tasksQueue), vt2(2, tasksQueue) {}

    void execute() {
        while (vt1.hasTasks() || vt2.hasTasks()) {
            if (vt1.hasTasks()) {
                if (!vt1.isWorking()) vt2.increaseCurrentIndex();
                vt1.runNextTask();
            }

            if (vt2.hasTasks()) {
                if (vt2.isWorking()) vt1.increaseCurrentIndex();
                vt2.runNextTask();
            }
        }
    }
};

#endif