#include <string>
#include <iostream>
#include "src/servers.hpp"
// #include "src/request.hpp"
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
#include <streambuf>
#include <sys/types.h>
#include <dirent.h>
// #include "servers.hpp"



int main(int argc, char *argv[])
{
    
    struct stat buffer;
    int i = stat(argv[1], &buffer);
    signal(SIGPIPE, SIG_IGN);

    // int set = 1;
// setsockopt(sd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
    if (i == -1)
    {
        std::cout << "Error : file not found" << std::endl;
        return 0;
    }
	httpServers srvrs(argc, argv);
	srvrs.httpServers_repl();
    // struct stat s;
    // if (stat("./root/hangover-master/", &s) == 0)
    // {
    //     if (s.st_mode & S_IFDIR)
    //     {
    //         std::cout << "is a directory" << std::endl;
    //     }
    // }
    // else
    // {
    //     std::cout << "is not a directory" << std::endl;
    // }
    // server s(4200);
    // s.start();
    // s.run();
    // std::stringstream ss;
    // std::string s;
    // std::ifstream file;
    // file.open("test.txt", std::fstream::in);

    // std::string line;
    // while (std::getline(file, line))
       // s.append(line + "\n");
        //ss << line << std::endl;
    // file.seekg(0, std::ios::end);
    // s.reserve(file.tellg());
    // file.seekg(0, std::ios::beg);
    // s.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    //std::cout << s << std::endl;
    // std::cout << request::is_request_complete(s);
    // std::cout << s.substr(s.find("Content-Length")) << std::endl;
   // int cl;
   // cl << skipColonNSpace("Content-Length: 260632");
    // std::cout << skipColonNSpace("Content-Length: 260632");;
    // ss << file;

    
    // std::cout << ss << std::endl;
    system("leaks webserver");
    return (0);
}
