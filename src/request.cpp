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
bool request::is_request_complete(std::string buffer)
{
    std::string content_length_s = "";
    size_t content_length = 0;


    if (buffer.find("Content-Length:") == std::string::npos)
        return false;
    if (buffer.find("\n\n") == std::string::npos)
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
        buffer = buffer.substr(buffer.find("\n\n") + 2);
    if (buffer.length() < content_length || buffer.length() > content_length)
        return false;
    return (true);
}

//get size of header
//get content length
// keep track of the total size - header size if equals content_length know you're done
