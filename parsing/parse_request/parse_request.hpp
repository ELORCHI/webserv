
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
        std::string							_headers_part;
        std::string							_body_part;
        std::string							_http_version;
        std::string							_http_method;
        std::string							_path_body;
        unsigned int						_port_request;
        std::map<std::string, std::string>	_http_path;
        std::map<std::string, std::string>	_http_headers;
    public:
        parse_request();
        ~parse_request();
        void set_lines(char *buffer);
        std::string set_http_vmp(std::string);    
        void set_http_method(std::string&);  
        void set_http_version(std::string&);  
        void set_http_path(std::string http_path);
        void set_http_headers(std::string&);
        void set_http_body(std::string&);
        void set_chunked_http_body(std::ifstream , std::ofstream );
        void set_unchunked_http_body(std::ifstream , std::ofstream );
        void eraseAllSubStr(std::string&, const std::string&);
        size_t hex_to_dec(std::string num);

        //getters
        std::string get_http_method() const {return _http_method;};  
        std::string get_http_version() const {return _http_version;};  
        std::string get_http_path() {return _http_path["path"];};
        std::string get_http_query() {return _http_path["query"];};
        std::map<std::string, std::string> get_http_headers() const {return _http_headers;};
        std::string get_http_body() const {return _http_body;};

        //methods
        bool	is_number(const std::string&);
        bool	is_hexnumber(const std::string&);
        void	start_parsing(std::string &, std::string &);
};




#endif