// GatewayServer.hpp
#ifndef GATEWAY_SERVER_HPP
#define GATEWAY_SERVER_HPP

#include "RequestHandler.hpp"
#include <boost/asio.hpp>

class GatewayServer {
public:
    GatewayServer();
    void run();

private:
    boost::asio::io_context io_context;
    RequestHandler request_handler;
    // Add more attributes for server configuration if needed
};

#endif // GATEWAY_SERVER_HPP
