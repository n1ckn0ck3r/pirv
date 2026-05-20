#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <semaphore>
#include <chrono>
#include <atomic>
#include <string>

struct PrintJob {
    std::string doc;
    int priority;
    int timeout_ms;
    int id;

    bool operator<(const PrintJob& other) const {
        return priority < other.priority;
    }
};

class PrinterQueue {
private:
    int n_printers;
    std::counting_semaphore<> semaphore;
    std::mutex mtx;
    std::priority_queue<PrintJob> job_queue;

public:
    PrinterQueue(int n) : n_printers(n), semaphore(n) {}

    void addJob(const std::string& doc, int priority, int timeout_ms, int id) {
        std::lock_guard<std::mutex> lock(mtx);
        job_queue.push({doc, priority, timeout_ms, id});

        std::cout << "Added job: " << doc << " with priority " << priority << " and timeout " << timeout_ms << "ms" << std::endl;
    }

    void printJob() {
        while (true) {
            PrintJob job;
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (job_queue.empty()) {
                    return;
                }
                job = job_queue.top();
                job_queue.pop();

                std::cout << "Processing job: " << job.doc << " with priority " << job.priority << " and timeout " << job.timeout_ms << "ms" << std::endl;
            }

            if (!semaphore.try_acquire_for(std::chrono::milliseconds(job.timeout_ms))) {
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    std::cout << "Job " << job.doc << " timed out and is removed from the queue" << std::endl;
                    job_queue.push(job);
                }

                std::this_thread::yield();
                continue;
            }

            {
                std::lock_guard<std::mutex> lock(mtx);
                std::cout << "Job " << job.doc << " is being printed" << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate printing time

            {
                std::lock_guard<std::mutex> lock(mtx);
                std::cout << "Job " << job.doc << " has been printed" << std::endl;
            }

            semaphore.release();
            std::this_thread::yield();
        }
    }
};

int main() {
    PrinterQueue pq(2);

    pq.addJob("Document1", 1, 500, 1);
    pq.addJob("Document2", 2, 300, 2);
    pq.addJob("Document3", 1, 400, 3);
    pq.addJob("Document4", 3, 200, 4);
    pq.addJob("Document5", 2, 600, 5);

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        std::thread t([&pq]() {
            pq.printJob();
        });

        t.detach();
        std::this_thread::yield();
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}