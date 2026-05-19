#include <iostream>
#include <vector>
#include <random>
#include <mutex>
#include <semaphore>
#include <thread>
#include <chrono>

std::counting_semaphore<6> cranes(5);
std::mutex mtx;

int waiting_trucks = 0;
int loaded_trucks = 0;
bool backup_crane_active = false;

void truck(int id) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        waiting_trucks++;
        std::cout << "Грузовик " << id << " прибыл. В очереди: " << waiting_trucks << " грузовиков." << std::endl;
        if (waiting_trucks >= 5 && !backup_crane_active) {
            backup_crane_active = true;
            cranes.release();
            std::cout << "В очереди больше пяти грузовиков, запущен резервный кран." << std::endl;
        }
    }

    cranes.acquire();
    {
        std::lock_guard<std::mutex> lock(mtx);
        waiting_trucks--;
        std::cout << "Грузовик " << id << " нашёл кран. Ожидают: " << waiting_trucks << " грузовиков." << std::endl;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(3, 6);

    int load_time = dist(gen);

    {
        std::lock_guard<std::mutex> lock(mtx);
        if (loaded_trucks < 3) {
            std::cout << "Активирован аварийный режим для грузовика " << id << " (загруженных меньше трёх). Скорость загрузки увеличена в два раза." << std::endl;
            load_time /= 2;
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(load_time));
    {
        std::lock_guard<std::mutex> lock(mtx);
        loaded_trucks++;
        std::cout << "Грузовик " << id << " загружен за " << load_time << " секунд. Всего в порту загружено " << loaded_trucks << " грузовиков." << std::endl;
    }

    cranes.release();
}

int main() {
    std::vector<std::thread> trucks;

    for (int i = 1; i <= 10; i++) {
        trucks.emplace_back(std::thread(truck, i));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    for (auto& t : trucks) {
        t.join();
    }

    std::cout << "Все грузовики были загружены." << std::endl;
    return 0;
}