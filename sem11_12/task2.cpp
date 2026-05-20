#include <iostream>
#include <string>
#include <variant>
#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>

using namespace boost::asio::experimental::awaitable_operators;
using boost::asio::ip::tcp;

boost::asio::awaitable<std::string> read_from(tcp::socket& socket, std::string name) {
    char data[1024];
    auto [ec, n] = co_await socket.async_read_some(boost::asio::buffer(data), boost::asio::as_tuple(boost::asio::use_awaitable));

    if (ec) {
        co_return "Error reading from " + name + ": " + ec.message();
    }

    co_return std::string(data, n);
}

boost::asio::awaitable<void> multiplexer(tcp::socket& socket1, tcp::socket& socket2) {
    for (;;) {
        auto result = co_await (read_from(socket1, "Socket 1") || read_from(socket2, "Socket 2"));

        std::visit([](auto& arg) {
            std::cout << arg << std::endl;
        }, result);
    }
}

int main() {
    try {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor1(io_context, tcp::endpoint(tcp::v4(), 12345));
        tcp::acceptor acceptor2(io_context, tcp::endpoint(tcp::v4(), 12346));

        std::cout << "Waiting for clients on ports 12345 and 12346..." << std::endl;
        boost::asio::co_spawn(io_context, [&]() -> boost::asio::awaitable<void> {
            tcp::socket socket1 = co_await acceptor1.async_accept(boost::asio::use_awaitable);
            tcp::socket socket2 = co_await acceptor2.async_accept(boost::asio::use_awaitable);
            std::cout << "Clients connected on both ports." << std::endl;
            co_await multiplexer(socket1, socket2);
        }, boost::asio::detached);

        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}