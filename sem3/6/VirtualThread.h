#ifndef VIRTUALTHREAD_H
#define VIRTUALTHREAD_H
#include <iostream>

class VirtualThread {
private:
    int arr[4] = {5, 7, 10, 12};
    int id;
    short currentIndex;
    
    long long factorial(int n) {
        if (n == 1 || n == 0) return 1;
        
        return n * factorial(n - 1);
    }
public:
    VirtualThread(int id) : id(id), currentIndex(0) {}

    bool hasTasks() {
        return currentIndex < 4;
    }

    void increaseCurrentIndex() {
        currentIndex++;
    }

    void run() {
        int n = arr[currentIndex];
        long long result = factorial(n);

        std::cout << "Виртуальный поток " << id << " вычисляет " << n << "! = " << result << std::endl;
        increaseCurrentIndex();
    }  
};

#endif