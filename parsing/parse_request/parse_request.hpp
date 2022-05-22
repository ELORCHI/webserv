
#ifndef PARSE_REQUEST_HPP
#define PARSE_REQUEST_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

class parse_request
{
    private:
        std::string _headers_part;
        std::string _body_part;
        std::string _http_version;
        std::string _http_method;
        unsigned int _port_request;
        // std::string _http_path;
        std::map<std::string, std::string> _http_path;
        std::map<std::string, std::string> _http_headers;
        std::string _http_body;
    public:
        parse_request();
        ~parse_request();
        //setters
        void set_lines(char *buffer);
        std::string set_http_vmp(std::string);    
        void set_http_method(std::string&);  
        void set_http_version(std::string&);  
        void set_http_path(std::string http_path);
        void set_http_headers(std::string&);
        void set_http_body(std::string&);
        void set_chunked_http_body(std::string&);
        void set_unchunked_http_body(std::string&);
        void eraseAllSubStr(std::string&, const std::string&);
        size_t hex_to_dec(std::string num);

        //getters
        // std::string get_http_version();
        // std::string get_http_method();
        // std::string get_http_path();
        // std::map<std::string, std::string> get_http_headers();
        // std::string get_http_body();
        // std::vector<std::string> get_lines();
        bool    is_number(const std::string&);
        bool    is_hexnumber(const std::string&);

        //methods
        void start_parsing(char *buffer);
};




#endif