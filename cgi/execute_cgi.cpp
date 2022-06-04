#include <stdlib.h>

execute_cgi::execute_cgi(/* args */)
{
}

execute_cgi::~execute_cgi()
{
}


std::string GetPortFromHeaders()
{
    std::string port;
    std::string tmp = get_http_headers().find("Host")->second;

    size_t found = tmp.find(":");
    if (found != std::string::npos)
        port = tmp.substr(found + 1);
    else
        port = "80";
}

void set_environement(parse_request request)
{
    setenv( "GATEWAY_INTERFACE", "CGI/1.1",0);
    setenv( "SERVER_PROTOCOL", "HTTP/1.1",0);
    setenv( "SERVER_PORT", GetPortFromHeaders().c_str(),0);
    setenv( "REMOTE_ADDR", "0.0.0.0",0);
    setenv( "PATH_INFO", get_http_path().c_str(),0);
    // setenv( "SCRIPT_FILENAME" , get_http_path()); //need full path
    setenv( "QUERY_STRING" , get_http_query().c_str(), 0);
    setenv( "REQUEST_METHOD" ,get_http_method().c_str(), 0);
    setenv( "REDIRECT_STATUS" , "true", 0);
    setenv( "FCGI_ROLE" , "RESPONDER", 0);
    setenv( "REQUEST_SCHEME" , "http", 0);
    setenv( "REMOTE_PORT" ,"0", 0);
    if (get_http_headers().find("Content-Lenght") != get_http_headers().end()) 
    {
        setenv( "CONTENT_TYPE", get_http_headers().find("Content-Type")->second.c_str(),0);
        setenv( "CONTENT_LENGTH", get_http_headers().find("Content-Lenght")->second.c_str(),0);
    }
    else 
    {
        setenv( "CONTENT_TYPE","text/html; charset=UTF-8" ,0);
        setenv( "CONTENT_LENGTH", "0" ,0);
    }
}


int start_execute_cgi(request request)
{
    pid_t   pid;
    int fd[2] = {-1};
    if (request.get_path_body() != "")
        fd[0] = open(request.get_path_body().c_str(), O_RDONLY);
    fd[1] = open("bodybuilding.html", O_RDWR | O_CREATE | O_TRUNC, 0644);

    if ((fd[0] == -1 && request.get_path_body() != "") || fd[1] == -1)
        return 500;
    pid = fork();
    if (pid == -1)
    {
        close(fd[1]);
        remove("bodybuilding.html");
        if (fd[0] > 0)
            close(fd[0]);
        return 500;
    }
    if (pid == 0)
    {
        int cgi_response_fd = 0;
        int body_fd = 0;
        set_environement(request);
        if (fd[0] > 0)
        {
            dup2(fd[0], 0);
            close(fd[0]); 
        }
        dup2(fd[1], 1);
        close(fd[1]);
        char *tmp[3];
        // tmp[0] = ; cgi_path
        // tmp[1] = ; file_full_path
        tmp[2] = NULL;
        int ret = execvp(tmp[0], tmp);
        remove("bodybuilding.html");
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
            status = waitpid(pid ,&stats, WNOHANG);
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
            remove("bodybuilding.html");
            kill(pid, SIGKILL);
            return 408;
        }
        
    }
}