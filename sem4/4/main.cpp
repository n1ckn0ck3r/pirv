#include "Logger.h"
#include <thread>
#include <random>
#include <string>

int main() {
    Logger<double> logger;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> norm(5, 10);
    
    for (int i = 0; i < 5; i++) {
        std::thread([&] (){
            logger.log(norm(gen));
        }).detach();
    }

    return 0;
}