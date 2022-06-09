#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// std::string GetPortFromHeaders(parse_request request)
// {
//     std::string port;
//     std::string tmp = request.get_http_headers().find("Host")->second;

//     size_t found = tmp.find(":");
//     if (found != std::string::npos)
//         port = tmp.substr(found + 1);
//     else
//         port = "80";
// }

void set_environement()
{
    setenv( "GATEWAY_INTERFACE", "CGI/1.1",1);
    setenv( "SERVER_PROTOCOL", "HTTP/1.1",1);
    setenv( "SERVER_PORT", "80",1);
    setenv( "REMOTE_ADDR", "0.0.0.0",1);
    setenv( "PATH_INFO", "/Users/yhebbat/Documents/GitHub/webserv/cgi/get.php",1);
    setenv( "SCRIPT_FILENAME" , "/Users/yhebbat/Documents/GitHub/webserv/cgi/get.php",1); //need full path
    setenv( "QUERY_STRING" , "firstname=test&lastname=test1",1);
    setenv( "REQUEST_METHOD" , "GET",1);///
    setenv( "REDIRECT_STATUS" , "true",1);
    setenv( "FCGI_ROLE" , "RESPONDER",1);
    setenv( "REQUEST_SCHEME" , "http",1);
    setenv( "REMOTE_PORT" ,"0",1);
    // if (request.get_http_headers().find("Content-Lenght") != request.get_http_headers().end()) 
    // {
    //     setenv( "CONTENT_TYPE", request.get_http_headers().find("Content-Type")->second.c_str(),1);
    //     setenv( "CONTENT_LENGTH", request.get_http_headers().find("Content-Lenght")->second.c_str(),1);
    // }
    // else 
    // {
        setenv( "CONTENT_TYPE","text/html; charset=UTF-8" ,1);
        setenv( "CONTENT_LENGTH", "0" ,1);
    // }
}


std::string gen_random(const int len) {
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

int main()
{
    pid_t   pid = -1;
    int fd[2] = {-1};
    // if (request.get_path_body() != "")
    //     fd[0] = open(request.get_path_body().c_str(), O_RDONLY);
    // if (_file_full_path == "")

    std::string    _file_full_path = gen_random(10) + ".html";
    fd[1] = open(_file_full_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd[1] == -1)
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
        set_environement();
        if (fd[0] > 0)
        {
            dup2(fd[0], 0);
            close(fd[0]); 
        }
        dup2(fd[1], 1);
        close(fd[1]);
        char *tmp[3];
        tmp[0] = "/Users/yhebbat/Documents/GitHub/webserv/cgi/php-cgi";//(char *)cgi_path.c_str(); //cgi_path
        tmp[1] = "/Users/yhebbat/Documents/GitHub/webserv/cgi/get.php";//(char *)file_full_path.c_str();// file_full_path
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
            status = waitpid(0 ,&stats, WNOHANG);
            if (status == 0)
                timeout = 1;
            else
            {
                timeout = 0;
                break ;
            }
        }
        sleep(1);
        if (timeout)
        {
            remove(_file_full_path.c_str());
            kill(pid, SIGKILL);
            return 408;
        }
        else
        {
            fd[1] = open(_file_full_path.c_str(), O_RDWR, 0644);
            char buffer[2000] = {0};
            std::string output;
		    int ret = 1;
            lseek(fd[1], 0, SEEK_SET);
            while (ret > 0)
            {
                memset(buffer, 0, 2000);
                ret = read(fd[1], buffer, 2000 - 1);
                output += buffer;
                if (output.find("\r\n\r\n") != std::string::npos)
                {
                    output = output.substr(0, output.find("\r\n\r\n"));
                    break;
                }
            }
            close(fd[1]);
            std::cout << "OUTPUT :  "<< output << std::endl;
        }
    }
}