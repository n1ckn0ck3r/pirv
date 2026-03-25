#include "Buffer.h"
#include <chrono>
#include <atomic>

int main() {
    Buffer<int> buffer(5);

    std::atomic<int> remaining_prods(3);
    std::atomic<bool> prod_finished(false);

    for (int i = 0; i < 3; i++) {
        int prod_id = i;
        std::thread([&buffer, prod_id, &remaining_prods]() {
            for (int j = 1; j < 5; j++) {
                int value = prod_id * 95 + j;
                buffer.produce(value);

                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                remaining_prods--;
                // std::cout << "Производитель " << std::this_thread::get_id() << " завершил работу" << std::endl;
            }
        }).detach();    
    }

    std::vector<std::thread> consumers;
    std::atomic<int> total_consumed(0);


    for (int i = 0; i < 2; i++) {
        consumers.emplace_back([&buffer, &total_consumed, &prod_finished] (){
            while (true) {
                if (buffer.get_count() == 0 && prod_finished) {
                    break;
                }

                int value = buffer.consume();
                total_consumed++;

                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                // std::cout << "Потребитель " << std::this_thread::get_id() << " завершил работу" << std::endl;
            }
        });
    }

    while (remaining_prods > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    prod_finished = true;

    for (auto& cons : consumers) {
        cons.join();
    }

    return 0;
}