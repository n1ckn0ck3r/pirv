#include <iostream>
#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        
        std::cout << "Сервер запущен. Ожидание подключений..." << std::endl;
    
        tcp::socket socket(io_context);
        acceptor.accept(socket);

        std::cout << "Клиент подключен." << std::endl;

        char data[1024];
        size_t length = socket.read_some(boost::asio::buffer(data));
        std::string client_message(data, length);

        std::string response = "Сообщение получено: " + client_message;
        boost::asio::write(socket, boost::asio::buffer(response));
    } catch (const std::exception& e) {
        std::cerr << "Ошибка сервера: " << e.what() << std::endl;
    }

    return 0;
}