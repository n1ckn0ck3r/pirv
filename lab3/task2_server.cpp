#include <iostream>
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>

using boost::asio::ip::tcp;

unsigned long long factorial(int n) {
    if (n < 0) return 0;
    if (n == 0 || n == 1) {
        return 1;
    }
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, boost::asio::thread_pool& pool) 
        : socket_(std::move(socket)), pool_(pool) {}

    void start() {
        do_read();
    }
private:
    tcp::socket socket_;
    boost::asio::thread_pool& pool_;
    char data_[1024];

    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string message(data_, length);
                    try {
                        int num = std::stoi(message);

                        boost::asio::post(pool_, [this, self, num]() {
                            unsigned long long result = factorial(num);
                            std::string response = "Факториал " + std::to_string(num) + " равен " + std::to_string(result);
                            do_write(response);
                        });
                    } catch (...) {
                        do_write("Ошибка: введите целое число.");
                    }
                }
            });
    }

    void do_write(const std::string& response) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(response),
            [this, self](boost::system::error_code ec, std::size_t) {
                if (!ec) {
                    do_read();
                }
            });
    }
};

class Server {
public:
    Server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), pool_(4) {
        do_accept();
    }

private:
    tcp::acceptor acceptor_;
    boost::asio::thread_pool pool_;
    
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket), pool_)->start();
                }
                do_accept();
            });
    }
};

int main() {
    try {
        boost::asio::io_context io_context;
        Server server(io_context, 12345);
        std::cout << "Сервер запущен. Ожидание подключений..." << std::endl;
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка сервера: " << e.what() << std::endl;
    }
}