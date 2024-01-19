// http_parser.cpp

#include "http_parser.hpp"

HttpRequest HttpRequest::parse(const std::string& raw_request) {
    HttpRequest request;
    std::istringstream stream(raw_request);
    std::string line;

    // Parse request line
    std::getline(stream, line);
    std::istringstream request_line(line);
    request_line >> request.method >> request.uri >> request.http_version;

    // Parse headers
    while (std::getline(stream, line) && line != "\r" && line != "") {
        auto delimiter_pos = line.find(": ");
        auto name = line.substr(0, delimiter_pos);
        auto value = line.substr(delimiter_pos + 2);
        request.headers[name] = value;
    }

    // Parse body (if any)
    if (stream.rdbuf()->in_avail() > 0) {
        std::getline(stream, request.body);
    }

    return request;
}

std::string HttpResponse::toString() const {
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