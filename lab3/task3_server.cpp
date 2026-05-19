#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket) 
        : socket_(std::move(socket)), timer_(socket_.get_executor()) {}

    void start() {
        do_read();
    }

private:
    tcp::socket socket_;
    boost::asio::steady_timer timer_;
    std::array<char, 1024> data_;

    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    process_request(std::string(data_.data(), length));
                }
            });
    }

    void process_request(const std::string& request) {
        auto self(shared_from_this());
        const std::string timer_prefix = "таймер ";

        std::cout << "Получено от клиента: " << request << ". Длина сообщения " << request.length() << std::endl;

        if (request.find("таймер ") == 0) {
            std::cout << "Запуск таймера на " << request.substr(timer_prefix.length()) << " секунд." << std::endl;
            handle_timer(request.substr(timer_prefix.length()));
        } else {
            do_write("Неизвестная команда. Используйте 'таймер N'.");
        }
    }

    void handle_timer(const std::string& time) {
        try {
            int seconds = std::stoi(time);
            auto self(shared_from_this());

            timer_.expires_after(std::chrono::seconds(seconds));
            timer_.async_wait([this, self, seconds](const boost::system::error_code& ec) {
                do_write("Прошло " + std::to_string(seconds) + " секунд!");
            });
        } catch (...) {
            do_write("Ошибка: неверный формат числа для таймера.");
        }
    }

    void do_write(const std::string& message) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(message),
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
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket))->start();
            }
            do_accept();
        });
    }

    tcp::acceptor acceptor_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        Server s(io_context, 12345);
        std::cout << "Сервер запущен. Ожидание подключений..." << std::endl;
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка сервера: " << e.what() << std::endl;
    }

    return 0;
}