#ifndef HYPERTHREADINGSIMULATOR_H
#define HYPERTHREADINGSIMULATOR_H
#include "VirtualThread.h"

class HyperThreadingSimulator {
private:
    VirtualThread vt1;
    VirtualThread vt2;
public:
    HyperThreadingSimulator() : vt1(1), vt2(2) {}

    void execute() {
        while (vt1.hasTasks() || vt2.hasTasks()) {
            if (vt1.hasTasks()) {
                vt1.run();
                vt2.increaseCurrentIndex();
            }

            if (vt2.hasTasks()) {
                vt2.run();
                vt1.increaseCurrentIndex();
            }
        }
    }
};

#endif