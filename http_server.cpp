#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    HttpSession(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {
        readRequest();
        processRequest();
        writeResponse();
    }

private:
    tcp::socket socket_;
    boost::asio::streambuf request_;

    void readRequest() {
        boost::asio::read_until(socket_, request_, "\r\n\r\n");
    }

    void processRequest() {
        // Here you would parse the request and prepare an appropriate response.
        // This is a simplified placeholder for request processing.
    }

    void writeResponse() {
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 14\r\n\r\nHello, world!\n";
        boost::asio::write(socket_, boost::asio::buffer(response));
    }
};

class HttpServer {
public:
    HttpServer(boost::asio::io_context& io_context, short port) 
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        doAccept();
    }

private:
    tcp::acceptor acceptor_;

    void doAccept() {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<HttpSession>(std::move(socket))->start();
            }
            doAccept();
        });
    }
};

int main() {
    try {
        boost::asio::io_context io_context;
        HttpServer server(io_context, 8080);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}