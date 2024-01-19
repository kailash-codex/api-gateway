#include <iostream>
#include <sstream>
#include <string>
#include <map>

class HttpRequest {
public:
    std::string method;
    std::string uri;
    std::string http_version;
    std::map<std::string, std::string> headers;
    std::string body;

    static HttpRequest parse(const std::string& raw_request) {
        HttpRequest request;
        std::istringstream stream(raw_request);
        std::string line;

        // Parse request line
        std::getline(stream, line);
        std::istringstream request_line(line);
        request_line >> request.method >> request.uri >> request.http_version;

        // Parse headers
        while (std::getline(stream, line) && line != "\r") {
            auto delimiter_pos = line.find(": ");
            auto name = line.substr(0, delimiter_pos);
            auto value = line.substr(delimiter_pos + 2);
            request.headers[name] = value;
        }

        // Parse body (optional)
        if (request.headers.find("Content-Length") != request.headers.end()) {
            std::getline(stream, request.body);
        }

        return request;
    }
};

class HttpResponse {
public:
    std::string http_version;
    std::string status_code;
    std::string status_message;
    std::map<std::string, std::string> headers;
    std::string body;

    std::string toString() const {
        std::ostringstream stream;

        // Status line
        stream << http_version << " " << status_code << " " << status_message << "\r\n";

        // Headers
        for (const auto& header : headers) {
            stream << header.first << ": " << header.second << "\r\n";
        }

        // Empty line to separate headers and body
        stream << "\r\n";

        // Body
        stream << body;

        return stream.str();
    }
};