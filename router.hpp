// router.hpp

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <string>
#include <vector>
#include <functional>
#include "http_parser.hpp"

struct Route {
    std::string path;
    std::string method;
    std::function<void(const HttpRequest&, HttpResponse&)> handler;
};

class Router {
public:
    void addRoute(const std::string& method, const std::string& path, 
                  std::function<void(const HttpRequest&, HttpResponse&)> handler) {
        routes_.push_back({path, method, handler});
    }

    std::pair<bool, std::function<void(const HttpRequest&, HttpResponse&)>> findHandler(const HttpRequest& request) {
        for (const auto& route : routes_) {
            if (request.method == route.method && request.uri == route.path) {
                return {true, route.handler};
            }
        }
        return {false, nullptr}; // No matching route found
    }

private:
    std::vector<Route> routes_;
};

#endif // ROUTER_HPP