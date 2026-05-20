#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

boost::asio::awaitable<void> echo_session(tcp::socket socket) {
    char data[1024];
    try {
        for (;;) {
            auto [ec, n] = co_await socket.async_read_some(
                boost::asio::buffer(data), boost::asio::as_tuple(boost::asio::use_awaitable)
            );

            if (ec == boost::asio::error::eof) {
                std::cout << "Client disconnected." << std::endl;
                break;
            }

            if (ec) {
                throw boost::system::system_error(ec);
            }

            co_await boost::asio::async_write(socket, boost::asio::buffer(data, n), boost::asio::use_awaitable);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in echo session: " << e.what() << std::endl;
    }
}

boost::asio::awaitable<void> echo_server(tcp::acceptor acceptor) {
    for (;;) {
        tcp::socket socket = co_await acceptor.async_accept(boost::asio::use_awaitable);
        std::cout << "New client connected." << std::endl;

        auto executor = co_await boost::asio::this_coro::executor;

        boost::asio::co_spawn(executor, echo_session(std::move(socket)), boost::asio::detached);
    }
}

int main() {
    try {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        std::cout << "Echo server is running on port 12345..." << std::endl;

        boost::asio::co_spawn(io_context, echo_server(std::move(acceptor)), boost::asio::detached);
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}