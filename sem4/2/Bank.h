#ifndef BANK_H
#define BANK_H
#include "Account.h"
#include <deque>
#include <condition_variable>
#include <thread>
#include <algorithm>
#include <iostream>
#include <memory>
#include <chrono>

template <typename T>
class Bank {
private:
    std::deque<Account<T>> accounts;
    std::condition_variable cv;
    std::mutex mtx;

public:
    Bank(int n, T initial_balance) {
        for (int i = 0; i < n; i++) {
            accounts.emplace_back(initial_balance);
        }
    }
    
    inline void transfer(int from, int to, T amount) {
        if (from == to) return;

        Account<T>& from_acc = accounts[from];
        Account<T>& to_acc = accounts[to];

        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [&]() {
            std::lock_guard<std::mutex> from_lock(from_acc.mtx);
            return from_acc.unsafe_balance() >= amount;
        });

        // std::scoped_lock lock_accounts(from_acc.mtx, to_acc.mtx);

        if (from_acc.withdraw(amount)) {
            to_acc.deposit(amount);

            std::cout << "Поток " << std::this_thread::get_id() 
                        << " перевел " << amount << " от аккаунта " << from 
                        << " к аккаунту " << to << std::endl;
        }

        cv.notify_all();
    }

    T total_balance() {
        T total = 0;
        for (auto& acc : accounts) {
            std::lock_guard<std::mutex> lock(acc.mtx);
            total += acc.unsafe_balance();
        }

        return total;
    }
};

#endif