cmake_minimum_required(VERSION 3.10)
project(ApiGateway)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(ApiGateway
    src/main.cpp
    src/api/api_handler.cpp
    src/network/network_manager.cpp
    src/security/security_manager.cpp
    # Add other source files here
)