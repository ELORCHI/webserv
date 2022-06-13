#ifndef EXECUTE_CGI_HPP
#define EXECUTE_CGI_HPP

#include "../parsing/parse_request/parse_request.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>


class parse_request;

class execute_cgi
{
    private:
        std::string _file_full_path;
        std::string _file_body_path;
        std::map<std::string, std::string>	_headers;
    public:
        execute_cgi();
        ~execute_cgi();
        std::string gen_random(const int len);
        //setters
        void    set_file_full_path(std::string file_full_path) { _file_full_path = file_full_path;};
        void    set_file_body_path(std::string file_body_path) { _file_body_path = file_body_path;};
        void    set_headers(std::map<std::string, std::string> headers);
        //getters
        std::string    get_file_full_path() { return _file_full_path;};
        std::string    get_file_body_path() { return _file_body_path;};
        std::map<std::string, std::string>    get_headers() { return _headers;};
        //methods
        int start_execute_cgi(std::string, std::string, parse_request );
        void set_environement(parse_request request, std::string file_full_path);
        std::string GetPortFromHeaders(parse_request request);
        void    set_headers(std::string &line);
        std::string upper_fun(std::string blan , bool set_http);


};


#endif