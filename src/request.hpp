#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "../parsing/parse_request/parse_request.hpp"
#include "../parsing/parse_confile/parse_confile.hpp"



int skipColonNSpace(std::string astring);

class request {
    int port;
    std::string req_host;
    int req_port;
    parse_request request_parsing_data;
    server *server_parsed_data;
    public:
        request(std::string request_text, int port, server *server_parsed_data);
        ~request() {}
        parse_request get_request_parsing_data();
        std::string getHost();
        int getPort();
        // int get_req_port();
        std::string getReqHost();

    // static int get_content_length(std::string astring);
    public:
        int get_port();
        static int get_content_length(std::string astring);
        static bool is_request_complete(std::string buffer);
        static bool is_chunked_request_complete(std::string buffer);
		static bool is_requestHeaderComplete(std::string buffer);
};
