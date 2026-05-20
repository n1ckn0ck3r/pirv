#include "TaskScheduler.hpp"

int main() {
    TaskScheduler scheduler(3);
    scheduler.submit(Task(1, 2, 500, 1));
    scheduler.submit(Task(2, 1, 300, 2));
    scheduler.submit(Task(3, 1, 400, 1));
    scheduler.submit(Task(4, 2, 200, 3));
    scheduler.submit(Task(5, 1, 600, 2));

    std::vector<std::thread> threads;

    for (int i = 0; i < 4; ++i) {
        std::thread t([&scheduler]() {
            scheduler.worker();
        });

        t.detach();
        std::this_thread::yield();
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    scheduler.stop();

    std::cout << "Total completed tasks: " << scheduler.get_completed_tasks() << std::endl;
    return 0;
}