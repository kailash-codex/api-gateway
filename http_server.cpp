#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <functional>
#include "http_parser.hpp" // Ensure this includes the definitions for HttpRequest and HttpResponse
#include "router.hpp"      // Ensure this includes the Route structure and Router class

using boost::asio::ip::tcp;

class HttpSession : public std::enable_shared_from_this<HttpSession>
{
public:
    HttpSession(tcp::socket socket, Router &router)
        : socket_(std::move(socket)), router_(router) {}

    void start()
    {
        readRequest();
    }

private:
    tcp::socket socket_;
    boost::asio::streambuf request_;
    Router &router_;

    void readRequest()
    {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, request_, "\r\n\r\n",
                                      [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                      {
                                          if (!ec)
                                          {
                                              processRequest();
                                          }
                                      });
    }

    void processRequest()
    {
        std::string raw_request(boost::asio::buffer_cast<const char *>(request_.data()), request_.size());
        HttpRequest parsed_request = HttpRequest::parse(raw_request);
        HttpResponse response;

        auto result = router_.findHandler(parsed_request);
        if (result.first)
        {
            result.second(parsed_request, response);
        }
        else
        {
            // No handler found: handle the 404 Not Found case
            response.http_version = "HTTP/1.1";
            response.status_code = "404";
            response.status_message = "Not Found";
            response.headers["Content-Type"] = "text/plain";
            response.body = "404 Not Found";
        }

        writeResponse(response);
    }

    void writeResponse(HttpResponse &response)
    {
        auto self(shared_from_this());
        auto response_data = std::make_shared<std::string>(response.toString());

        boost::asio::async_write(socket_, boost::asio::buffer(*response_data),
                                 [this, self, response_data](boost::system::error_code ec, std::size_t /*length*/)
                                 {
                                     if (!ec)
                                     {
                                         // Close the socket (TCP connection)
                                         boost::system::error_code ignored_ec;
                                         socket_.shutdown(tcp::socket::shutdown_both, ignored_ec);
                                     }
                                 });
    }
};

class HttpServer
{
public:
    HttpServer(boost::asio::io_context &io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    {
        doAccept();
    }

    void addRoute(const std::string &method, const std::string &path,
                  std::function<void(const HttpRequest &, HttpResponse &)> handler)
    {
        router_.addRoute(method, path, handler);
    }

private:
    tcp::acceptor acceptor_;
    Router router_;

    void doAccept()
    {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket)
                               {
            if (!ec) {
                std::make_shared<HttpSession>(std::move(socket), router_)->start();
            }
            doAccept(); });
    }
};

int main()
{
    try
    {
        boost::asio::io_context io_context;
        HttpServer server(io_context, 8080);

        // Setup routes using the public addRoute method
        server.addRoute("GET", "/example", [](const HttpRequest &req, HttpResponse &res)
                        {
            res.http_version = "HTTP/1.1";
            res.status_code = "200";
            res.status_message = "OK";
            res.headers["Content-Type"] = "text/plain";
            res.body = "This is an example response."; });

        // Add more routes as needed...

        io_context.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}