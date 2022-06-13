#include <stdlib.h>
#include "execute_cgi.hpp"

execute_cgi::execute_cgi():
    _file_full_path(""),
    _file_body_path(""),
    _headers()
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
    return port;
}

std::string execute_cgi::upper_fun(std::string blan , bool set_http)
{
    std::string tmp = "";
    if (set_http)
        tmp = "HTTP_";
    for (size_t i = 0; i < blan.size(); i++)
    {
        tmp += toupper(blan[i]);
    }
    return tmp;
}

void execute_cgi::set_environement(parse_request request, std::string file_full_path)
{
    setenv( "GATEWAY_INTERFACE", "CGI/1.1",1);
    setenv( "SERVER_PROTOCOL", "HTTP/1.1",1);
    setenv( "SERVER_PORT", GetPortFromHeaders(request).c_str(),1);
    setenv( "REMOTE_ADDR", "0.0.0.0",1);
    setenv( "PATH_INFO", request.get_http_path().c_str(),1);
    setenv( "SCRIPT_FILENAME" , file_full_path.c_str(),1); //need full path
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
    if (request.get_http_headers().size() > 0)
    {
        std::map <std::string, std::string> tmp = request.get_http_headers();
        for(std::map <std::string, std::string>::iterator it = tmp.begin(); it!=tmp.end(); ++it)
            setenv( upper_fun(it->first, 1).c_str() ,it->second.c_str() ,1);
    }
}

void    execute_cgi::set_headers(std::string &line)
{
	size_t found;
	line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::stringstream ss(line);
	std::string token;
	std::string key;
	std::string value;
	while (std::getline(ss, token, '\n'))
	{
		found = token.find(":");
		if (found != std::string::npos)
		{
			key = token.substr(0, found);
			value = token.substr(found + 2);
			key.erase(remove(key.begin(), key.end(), ' '), key.end());
			value.erase(remove(value.begin(), value.end(), ' '), value.end());
			_headers[key] = value;
		}
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
    // std::cout << "start_execute_cgi" << std::endl;
    // std::cout << "file_full_path :" << file_full_path << std::endl;
    // std::cout << "cgi_path :" << cgi_path << std::endl;
    // std::cout << "method : " << request.get_http_method() << std::endl;
    // std::cout << "query : " << request.get_http_query() << std::endl;
    // std::cout << "path : " << request.get_path_body() << std::endl;
    pid_t   pid = -1;
    int fd[2] = {-1};
    int last_fd = -1;
    if (request.get_path_body() != "")
        fd[0] = open(request.get_path_body().c_str(), O_RDONLY);
    if (_file_full_path == "")
    {
        _file_full_path = "/tmp/" + gen_random(10) + ".html";
        _file_body_path = "/tmp/" + gen_random(11) + ".html";
    }
    // std::cout << "***** file_full_path :" << _file_full_path << std::endl;
    // std::cout << "***** file_body_path :" << _file_body_path << std::endl;
    fd[1] = open(_file_full_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
    last_fd = open(_file_body_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
    if ((fd[0] == -1 && request.get_path_body() != "") || fd[1] == -1 || last_fd == -1)
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
        set_environement(request, file_full_path);
        if (fd[0] > 0)
        {
            // std::cout << "***** fd[0] :" << fd[0] << std::endl;
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
        exit(ret);// IHAVE TO RETURN BAD GATEWAY
        //dup to 1 && (dup to 0 if body is here)
    }
    else
    {
        time_t t = time(NULL);
        close(fd[1]);
        if (fd[0] > 0)
            close(fd[0]); 
        int status, stats, timeout;
        timeout = 0;
        while (difftime(time(NULL),  t) <= 60)
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
            // remove(_file_full_path.c_str());
            kill(pid, SIGKILL);
            return 408;
        }
        else
        {
            fd[1] = open(_file_full_path.c_str(), O_RDWR, 0644);
            if (fd[1] == -1)
                return 500;
            char buffer[2000] = {0};
            std::string output;
            std::string headers;
		    int ret = 1;
            lseek(fd[1], 0, SEEK_SET);
            bool headers_done = false;
            while (ret > 0)
            {
                memset(buffer, 0, 2000);
                ret = read(fd[1], buffer, 2000 - 1);
                output += buffer;
                if (output.find("\r\n\r\n") != std::string::npos && !headers_done)
                {
                    headers = output.substr(0, output.find("\r\n\r\n"));
                    // std::cout << "headers : " << headers << std::endl;
                    output = output.erase(0, output.find("\r\n\r\n") + 4);
                    headers_done = true;
                    set_headers(headers);
                }
                if (headers_done)
                {
                    int t = write(last_fd, output.c_str(), output.size());
                    output.clear();
                    if (t == -1)
                    {
                        // remove(_file_full_path.c_str());
                        close(fd[1]);
                        close(last_fd);
                        return 500;
                    }
                }
            }
            // remove(_file_full_path.c_str());
            close(fd[1]);
            close(last_fd);
            // for(std::map <std::string, std::string>::iterator it =_headers.begin(); it!=_headers.end(); ++it)
            // {
            //    std::cout << it->first << " => " << it->second << '\n';
            // }
        }
        return 1;
    }
}