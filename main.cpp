// main.cpp
#include "GatewayServer.hpp"

int main() {
    try {
        GatewayServer server;
        server.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}