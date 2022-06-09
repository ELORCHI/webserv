#include "request.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

std::vector<std::string> splitString(const std::string input, const char& delimiter) {

    std::vector<std::string> elements;
    std::stringstream stream(input);
    std::string element;

    while (getline(stream, element, delimiter)) {
        elements.push_back(element);
    }
    return elements;

}

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

// bool request::is_request_complete(std::string buffer)
// {
//     size_t content_length = 0;
//     std::string buffer_body;
//     if (buffer.find("\n\n") == std::string::npos)
//         return false;
//     if (buffer.find("Transfer-Encoding:") != std::string::npos)
//         return (is_chunked_request_complete(buffer));
//     if (buffer.find("Content-Length:") == std::string::npos)
//         return false;
//     std::string line;
//     std::stringstream ss(buffer);
//     while (std::getline(ss, line))
//     {
//         if (line.find("Content-Length:") != std::string::npos)
//         {
//             content_length = get_content_length(line);
//             continue;
//         }
//     }
//     if (content_length)
//         buffer_body = buffer.substr(buffer.find("\n\n") + 2);
//     if (buffer_body.length() < content_length || buffer_body.length() > content_length)
//         return false;
//     return (true);
// }

// std::string gen_random(const int len) {
//     srand((unsigned)time(NULL) * getpid());
//     static const char alphanum[] ="0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz";
//     std::string tmp_s;
//     tmp_s.reserve(len);
//     for (int i = 0; i < len; ++i) {
//         tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
//     }
//     return tmp_s;
// }

bool request::is_requestHeaderComplete(std::string buffer)
{	
     if (buffer.find("\r\n\r\n") != std::string::npos)
       	return true;
	// std::cout << "bruh" << std::endl;
	return false;
}

//     size_t content_length = 0;
//     std::string buffer_body;
//     if (buffer.find("\n\n") == std::string::npos)
//         return false;
//     if (buffer.find("Transfer-Encoding:") != std::string::npos)
//         return (is_chunked_request_complete(buffer));
//     if (buffer.find("Content-Length:") == std::string::npos)
//         return false;
//     std::string line;
//     std::stringstream ss(buffer);
//     while (std::getline(ss, line))
//     {
//         if (line.find("Content-Length:") != std::string::npos)
//         {
//             content_length = get_content_length(line);
//             continue;
//         }
//     }
//     if (content_length)
//         buffer_body = buffer.substr(buffer.find("\n\n") + 2);
//     if (buffer_body.length() < content_length || buffer_body.length() > content_length)
//         return false;
//     return (true);
// }
request::request(std::string request_text, int port, server *server_parsed_data)
{
    parse_request pr;
    char *_request_text = (char *)request_text.c_str();
    pr.start_parsing(_request_text);
    request_parsing_data = pr;
    std::string Host = request_parsing_data.get_http_headers()["Host"];
    if (Host.find(":") != std::string::npos) {
        std::vector<std::string> sv = splitString(request_parsing_data.get_http_headers()["Host"], ':');
        req_host = sv[0];
        req_port = atoi(sv[1].c_str());
    }
    else
    {
        req_host = Host;
        req_port = 80;
    }
    this->server_parsed_data = server_parsed_data;
}

parse_request request::get_request_parsing_data()
{
    return (request_parsing_data);
}

std::string request::getHost()
{
    return (req_host);
}

int request::getPort()
{
    return (req_port);
}