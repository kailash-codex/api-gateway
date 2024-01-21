// RequestHandler.h
#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>

class RequestHandler {
public:
    RequestHandler();
    std::string handleRequest(const std::string& request);

private:
    // Add private methods or variables if needed
};

#endif // REQUEST_HANDLER_H