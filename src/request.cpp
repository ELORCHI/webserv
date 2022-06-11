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


bool request::is_chunked_request_complete(std::fstream &body)
{
	std::string buffer;
	std::stringstream ss;
	while (getline(body, buffer))
	{
		ss << buffer;
		if (buffer.find("0\r\n\r\n") != std::string::npos)
			return true;
	}
	return false;
}

// bool request::is_chunked_request_complete(std::string buffer)
// {
// 	//find the end of the chunked request
// 	if (buffer.find("0\r\n\r\n") != std::string::npos)
// 		return true;
// 	return false;
// }

    //if (buffer.find("0\r\n\r\n") != std::string::npos)
        // return true;
    // return false;
//}

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

bool request::is_request_complete(std::string headers, std::fstream &body)
{
	// std::cout << "im here" << std::endl;
	size_t content_length = 0;
	size_t body_size = 0;
	std::string buffer_body;
	// if (headers.find("\n\n") == std::string::npos)
	// {
	// 	return false;
	// }
	// std::cout << "body size: " <<  body_size << std::endl;
	if (headers.find("Transfer-Encoding:") != std::string::npos)
		return (is_chunked_request_complete(body));
	if (headers.find("Content-Length:") == std::string::npos)
		return false;
	std::string line;
	std::stringstream ss(headers);
	while (std::getline(ss, line))
	{
		if (line.find("Content-Length:") != std::string::npos)
		{
			content_length = get_content_length(line);
			// std::cout << "content_length: " << content_length << std::endl;
			continue;
		}
	}
	if (content_length)
	{
		body.seekg(0, std::ios::end);
		body_size = body.tellg();
		body.seekg(0, std::ios::beg);
	}
		// std::cout << "content_length: " << content_length << std::endl;
		// std::cout << "body_size: " << body_size << std::endl;
	if (body_size < content_length || body_size > content_length)
	{
		return false;
	}
	return (true);
}

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
request::request(parse_request &pr, server *server_parsed_data)
{
    // char *_request_text = (char *)request_text.c_str();

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