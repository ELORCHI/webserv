#include <stdlib.h>
#include "execute_cgi.hpp"

execute_cgi::execute_cgi(/*  */):
    _file_full_path("")
{
}

execute_cgi::~execute_cgi()
{
}


std::string execute_cgi::GetPortFromHeaders(parse_request request)
{
    std::string port;
    std::string tmp = request.get_http_headers().find("Host")->second;

    size_t found = tmp.find(":");
    if (found != std::string::npos)
        port = tmp.substr(found + 1);
    else
        port = "80";
}

void execute_cgi::set_environement(parse_request request)
{
    setenv( "GATEWAY_INTERFACE", "CGI/1.1",1);
    setenv( "SERVER_PROTOCOL", "HTTP/1.1",1);
    setenv( "SERVER_PORT", GetPortFromHeaders(request).c_str(),1);
    setenv( "REMOTE_ADDR", "0.0.0.0",1);
    setenv( "PATH_INFO", request.get_http_path().c_str(),1);
    setenv( "SCRIPT_FILENAME" , file_full_path,1); //need full path
    setenv( "QUERY_STRING" , request.get_http_query().c_str(),1);
    setenv( "REQUEST_METHOD" , request.get_http_method().c_str(),1);
    setenv( "REDIRECT_STATUS" , "true",1);
    setenv( "FCGI_ROLE" , "RESPONDER",1);
    setenv( "REQUEST_SCHEME" , "http",1);
    setenv( "REMOTE_PORT" ,"0",1);
    if (request.get_http_headers().find("Content-Lenght") != request.get_http_headers().end()) 
    {
        setenv( "CONTENT_TYPE", request.get_http_headers().find("Content-Type")->second.c_str(),1);
        setenv( "CONTENT_LENGTH", request.get_http_headers().find("Content-Lenght")->second.c_str(),1);
    }
    else 
    {
        setenv( "CONTENT_TYPE","text/html; charset=UTF-8" ,1);
        setenv( "CONTENT_LENGTH", "0" ,1);
    }
}


std::string execute_cgi::gen_random(const int len) {
	srand((unsigned)time(NULL) * getpid());     
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);
    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return tmp_s;
}

int execute_cgi::start_execute_cgi(std::string file_full_path, std::string cgi_path, parse_request request)
{
    pid_t   pid = -1;
    int fd[2] = {-1};
    if (request.get_path_body() != "")
        fd[0] = open(request.get_path_body().c_str(), O_RDONLY);
    if (_file_full_path == "")
        _file_full_path = "./upload/" + gen_random(10) + ".html";
    fd[1] = open(_file_full_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
    if ((fd[0] == -1 && request.get_path_body() != "") || fd[1] == -1)
        return 500;
    pid = fork();
    if (pid == -1)
    {
        close(fd[1]);
        if (_file_full_path != "")
            remove(_file_full_path.c_str());
        if (fd[0] > 0)
            close(fd[0]);
        return 500;
    }
    if (pid == 0)
    {
        set_environement(request);
        if (fd[0] > 0)
        {
            dup2(fd[0], 0);
            close(fd[0]); 
        }
        dup2(fd[1], 1);
        close(fd[1]);
        char *tmp[3];
        tmp[0] = (char *)cgi_path.c_str(); //cgi_path
        tmp[1] = (char *)file_full_path.c_str();// file_full_path
        tmp[2] = NULL;
        int ret = execvp(tmp[0], tmp);
        std::cout << "execve failed: " << strerror(errno) << std::endl;
        remove(_file_full_path.c_str());
        exit(ret);
        //dup to 1 && (dup to 0 if body is here)
    }
    else
    {
        time_t t(NULL);
        close(fd[1]);
        if (fd[0] > 0)
            close(fd[0]); 
        int status, stats, timeout;
        timeout = 0;
        while (difftime(time(NULL),  t) <= 5)
        {
            status = waitpid(pid ,&stats, 0);
            if (status == 0)
                timeout = 1;
            else
            {
                timeout = 0;
                break ;
            }
        }
        if (timeout)
        {
            remove(_file_full_path.c_str());
            kill(pid, SIGKILL);
            return 408;
        }
    
    }
}