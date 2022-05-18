#include "request.hpp"
#include <iostream>
#include <sstream>

int request::get_content_length(std::string astring)
{
    std::stringstream ss;
    int res;
    astring = astring.substr(astring.find(":") + 1);
    ss << astring;
    ss >> res;
    return res;
}

bool request::is_chunked_request_complete(std::string buffer)
{
    if (buffer.find("0\r\n\r\n") != std::string::npos)
        return true;
    return false;
}

bool request::is_request_complete(std::string buffer)
{
    size_t content_length = 0;
    std::string buffer_body;
    if (buffer.find("\n\n") == std::string::npos)
        return false;
    if (buffer.find("Transfer-Encoding:") != std::string::npos)
        return (is_chunked_request_complete(buffer));
    if (buffer.find("Content-Length:") == std::string::npos)
        return false;
    std::string line;
    std::stringstream ss(buffer);
    while (std::getline(ss, line))
    {
        if (line.find("Content-Length:") != std::string::npos)
        {
            content_length = get_content_length(line);
            continue;
        }
    }
    if (content_length)
        buffer_body = buffer.substr(buffer.find("\n\n") + 2);
    if (buffer_body.length() < content_length || buffer_body.length() > content_length)
        return false;
    return (true);
}




