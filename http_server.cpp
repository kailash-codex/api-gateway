#include <boost/asio.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

using boost::asio::ip::tcp;

// Include the HTTP parser header (assuming it's named http_parser.hpp)
#include "http_parser.cpp" 

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    HttpSession(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {
        readRequest();
        processRequest();
    }

    void writeResponse(const HttpResponse& response) {
        std::string response_str = response.toString();
        boost::asio::write(socket_, boost::asio::buffer(response_str));
    }

private:
    tcp::socket socket_;
    boost::asio::streambuf request_;

    void readRequest() {
        boost::asio::read_until(socket_, request_, "\r\n\r\n");
    }

    void processRequest() {
        // Convert the request buffer into a string
        std::string raw_request(boost::asio::buffer_cast<const char*>(request_.data()), request_.size());

        // Use the HttpRequest class from http_parser.hpp to parse the request
        HttpRequest parsed_request = HttpRequest::parse(raw_request);

        // Process the parsed request
        // (This is where you'd add your custom logic based on the request)
        // ...

        // Prepare the response (this is a simple static response for demonstration)
        HttpResponse response;
        response.http_version = "HTTP/1.1";
        response.status_code = "200";
        response.status_message = "OK";
        response.headers["Content-Type"] = "text/html";
        response.body = "<html><body><h1>Response from C++ HTTP Server</h1></body></html>";

        // Convert the response to a string and send
        std::string response_string = response.toString();
        boost::asio::write(socket_, boost::asio::buffer(response_string));

        writeResponse(response);

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