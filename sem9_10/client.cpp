#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Client : public std::enable_shared_from_this<Client> {
public:
    Client(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints)
        : socket_(io_context) {
        do_connect(endpoints);
    }

    void send_message(const std::string& message) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(message),
            [this, self](boost::system::error_code ec, size_t /*length*/) {
                if (!ec) {
                    std::cout << "Сообщение отправлено." << std::endl;
                    do_read();
                } else {
                    std::cout << "Ошибка отправки: " << ec.message() << std::endl;
                }
            });
    }

private:
    void do_connect(const tcp::resolver::results_type& endpoints) {
        auto self(shared_from_this());
        boost::asio::async_connect(socket_, endpoints,
            [this, self](boost::system::error_code ec, tcp::endpoint) {
                if (!ec) {
                    std::cout << "Клиент подключился к серверу." << std::endl;
                } else {
                    std::cout << "Ошибка подключения: " << ec.message() << std::endl;
                }
            });
    }

    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, size_t length) {
                if (!ec) {
                    std::cout << "Получено от сервера: " << std::string(data_, length) << std::endl;
                } else {
                    std::cout << "Ошибка приёма: " << ec.message() << std::endl;
                }
            });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

int main() {
    try {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "12345");

        auto client = std::make_shared<Client>(io_context, endpoints);

        std::thread t([&io_context]() { io_context.run(); });

        std::string message;
        while (true) {
            std::cout << "Введите координаты (например, 55.75,37.61): ";
            std::getline(std::cin, message);

            if (message.empty()) {
                break;
            }

            client->send_message(message);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        io_context.stop();
        t.join();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка " << e.what() << std::endl;
        return 1;
    }

    return 0;
}