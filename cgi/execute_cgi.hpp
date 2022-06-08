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
    public:
        execute_cgi();
        ~execute_cgi();
        std::string gen_random(const int len);
        int execute_cgi::start_execute_cgi(std::string, std::string, parse_request );
        void set_environement(parse_request request);
        std::string GetPortFromHeaders(parse_request request);
};


#endif