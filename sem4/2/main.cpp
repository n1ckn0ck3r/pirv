#include "Bank.h"
#include <random>

int main() {
    int num_accounts = 5;
    int num_threads = 10;
    int initial_balance = 10000;
    int operations = 100;

    Bank<int> bank(num_accounts, initial_balance);

    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back([&bank, operations]() {
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<> acc_dist(0, 4);
            std::uniform_int_distribution<> amount_dist(1, 100);

            for (int j = 0; j < operations; j++) {
                int from = acc_dist(gen);
                int to = acc_dist(gen);
                int amount = amount_dist(gen);

                bank.transfer(from, to, amount);
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Общий баланс " << bank.total_balance() << std::endl;

    return 0;
}