#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <random>
#include <stdexcept>
#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>

using namespace boost::asio::experimental::awaitable_operators;

class BankAccount {
private:
    int balance_;
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;

public:
    BankAccount(boost::asio::io_context& io_context) : balance_(0), strand_(boost::asio::make_strand(io_context.get_executor())) {}

    boost::asio::awaitable<void> async_deposit(int amount) {
        co_await boost::asio::post(strand_, boost::asio::use_awaitable);

        balance_ += amount;
        co_return;
    }

    boost::asio::awaitable<void> async_withdraw(int amount) {
        co_await boost::asio::post(strand_, boost::asio::use_awaitable);

        if (balance_ < amount) {
            throw std::invalid_argument("Insufficient funds");
        }
        balance_ -= amount;
        co_return;
    }

    boost::asio::awaitable<int> async_get_balance() {
        co_await boost::asio::post(strand_, boost::asio::use_awaitable);
        co_return balance_;
    }
}; 

boost::asio::awaitable<void> worker_session(BankAccount& account, std::atomic<int>& total_deposited, std::atomic<int>& total_withdrawn) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> amount_dist(1, 1000);
    std::uniform_int_distribution<> action_dist(0, 1);

    int local_deposited = 0;
    int local_withdrawn = 0;

    for (int i = 0; i < 20; ++i) {
        int amount = amount_dist(gen);
        bool is_deposit = action_dist(gen) == 0;

        if (is_deposit) {
            co_await account.async_deposit(amount);
            local_deposited += amount;
        } else {
            try {
                co_await account.async_withdraw(amount);
                local_withdrawn += amount;
            } catch (const std::invalid_argument& e) {}
        }
        
        boost::asio::steady_timer timer(co_await boost::asio::this_coro::executor);
        timer.expires_after(std::chrono::microseconds(50));
        co_await timer.async_wait(boost::asio::use_awaitable);
    }

    total_deposited += local_deposited;
    total_withdrawn += local_withdrawn;
}

int main() {
    try {
        boost::asio::io_context io_context;

        BankAccount account(io_context);

        std::atomic<int> total_deposited(0);
        std::atomic<int> total_withdrawn(0);
        std::atomic<int> active_coroutines(100);

        for (int i = 0; i < 100; ++i) {
            boost::asio::co_spawn(io_context, worker_session(account, total_deposited, total_withdrawn),
                [&active_coroutines](std::exception_ptr ep) {
                    if (ep) {
                        try {
                            std::rethrow_exception(ep);
                        } catch (const std::exception& e) {
                            std::cerr << "Coroutine error: " << e.what() << std::endl;
                        }
                    }
                    --active_coroutines;
                });
        }

        std::vector<std::thread> threads;
        for (int i = 0; i < 4; ++i) {
            threads.emplace_back([&io_context]() {
                io_context.run();
            });
        }

        while (active_coroutines > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        io_context.stop();

        for (auto& t : threads) {
            t.join();
        }

        int final_balance = 0;
        boost::asio::co_spawn(io_context, account.async_get_balance(),
            [&final_balance](std::exception_ptr, int balance) {
                if (balance) {
                    final_balance = balance;
                }
            });

        io_context.restart();
        io_context.run();

        int expected_balance = total_deposited.load() - total_withdrawn.load();
        std::cout << "Simulation completed." << std::endl;
        std::cout << "Total Deposited: " << total_deposited.load() << std::endl;
        std::cout << "Total Withdrawn: " << total_withdrawn.load() << std::endl;
        std::cout << "Expected Final Balance: " << expected_balance << std::endl;
        std::cout << "Actual Final Balance: " << final_balance << std::endl;

        if (final_balance == expected_balance) {
            std::cout << "Test passed: Final balance matches expected balance." << std::endl;
        } else {
            std::cout << "Test failed: Final balance does not match expected balance." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}