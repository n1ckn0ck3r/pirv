#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "12345");
        boost::asio::connect(socket, endpoints);

        while (true) {
            std::string message;
            std::cout << "Введите число для вычисления факториала: ";
            std::cin >> message;
            message += "\n";

            boost::asio::write(socket, boost::asio::buffer(message));

            boost::asio::streambuf response;
            boost::asio::read_until(socket, response, "\n");
            std::cout << "Ответ от сервера: " << &response << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка клиента: " << e.what() << std::endl;
    }

    return 0;
}