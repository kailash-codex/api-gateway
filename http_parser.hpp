// http_parser.hpp

#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include <string>
#include <map>
#include <sstream>

// Represents an HTTP request
class HttpRequest {
public:
    std::string method;
    std::string uri;
    std::string http_version;
    std::map<std::string, std::string> headers;
    std::string body;

    static HttpRequest parse(const std::string& raw_request);
};

// Represents an HTTP response
class HttpResponse {
public:
    std::string http_version;
    std::string status_code;
    std::string status_message;
    std::map<std::string, std::string> headers;
    std::string body;

    std::string toString() const;
};

#endif // HTTP_PARSER_HPP