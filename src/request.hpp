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
        request(parse_request &pr, server *server_parsed_data);
        ~request() {delete server_parsed_data;}
        parse_request get_request_parsing_data();
        std::string getHost();
        int getPort();
        // int get_req_port();
        std::string getReqHost();
        server *get_server(void)
        {
            return server_parsed_data;
        }
        void set_server(server *server_parsed_data)
        {
            this->server_parsed_data = server_parsed_data;
        }

    // static int get_content_length(std::string astring);
    public:
        //int get_port();
        static int get_content_length(std::string astring);
        static bool is_chunked_request_complete(std::fstream &body);
		static bool is_requestHeaderComplete(std::string buffer);
        static bool is_request_complete(std::string headers, std::fstream &body);
        static bool is_request_complete_(std::string headers, long long size, char *buffer);
		static std::string get_header_value(std::string headers, std::string header_name);
};
