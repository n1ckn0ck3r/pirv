#include <iostream>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;
std::atomic<int> curr_stage(0);

void stage (int id) {
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [&]() { return id == curr_stage; });

    std::cout << "Выполняется этап " << id << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "Этап " << id << " завершён" << std::endl;;

    curr_stage.fetch_add(1);
    lock.unlock();
    cv.notify_all();
}

int main() {
    std::thread t1(stage, 0);
    std::thread t2(stage, 1);
    std::thread t3(stage, 2);
    std::thread t4(stage, 3);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}