#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <mutex>

template <typename T>
class Account {
private:
    T balance;

public:
    std::mutex mtx;

    Account(T balance) : balance(balance) {}

    Account(const Account&) = delete;
    Account& operator=(const Account&) = delete;

    Account(Account&&) = default;
    Account& operator=(Account&&) = default;

    T get_balance() {
        std::lock_guard<std::mutex> lock(mtx);
        return balance;
    }

    void deposit(T amount) {
        std::lock_guard<std::mutex> lock(mtx);
        balance += amount;
    }

    bool withdraw(T amount) {
        std::lock_guard<std::mutex> lock(mtx);
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }  

    T& unsafe_balance() {
        return balance;
    }
};

#endif