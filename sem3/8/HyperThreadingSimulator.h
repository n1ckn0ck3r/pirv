#ifndef HYPERTHREADINGSIMULATOR_H
#define HYPERTHREADINGSIMULATOR_H
#include "VirtualThread.h"
#include <random>

class HyperThreadingSimulator {
private:
    VirtualThread vt1;
    VirtualThread vt2;

public:
    HyperThreadingSimulator() {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<int> valueDist(1, 50);
        std::uniform_int_distribution<int> durationDist(200, 1000);
        std::uniform_int_distribution<int> priorityDist(1, 10);
        std::uniform_int_distribution<int> stepsDist(2, 5);

        for (int i = 0; i < 6; i++) {
            vt1.addTask(Task(valueDist(gen), priorityDist(gen), durationDist(gen), stepsDist(gen)));
            vt2.addTask(Task(valueDist(gen), priorityDist(gen), durationDist(gen), stepsDist(gen)));
        }
    }

    void execute() {
        while (vt1.hasTasks() || vt2.hasTasks()) {
            if (vt1.hasTasks()) vt1.runStep(1);
            if (vt2.hasTasks()) vt2.runStep(2);
        }
    }
};

#endif