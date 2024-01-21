// GatewayServer.cpp
#include "GatewayServer.hpp"
#include <iostream>

GatewayServer::GatewayServer() : request_handler() {
    // Initialize server settings
}

void GatewayServer::run() {
    std::cout << "Server is running..." << std::endl;
    
    // Example usage of RequestHandler
    std::string exampleRequest = "Hello, Gateway!";
    std::string response = request_handler.handleRequest(exampleRequest);
    std::cout << "Response: " << response << std::endl;

    io_context.run();
    // Add server logic here
}