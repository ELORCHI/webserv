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

int basic_error(std::string error_message, char const *av, std::string error_message2)
{
    if (av && error_message2 != "" && error_message != "")
        std::cout << error_message << av << error_message2 << std::endl;
    else if (av && error_message != "")
        std::cout << error_message << av << std::endl;
    else if (error_message != "")
        std::cout << error_message << std::endl;    
    return (0);
}

int main(int argc, char *argv[])
{
    
    struct stat buffer;
    int i = stat(argv[1], &buffer);
    signal(SIGPIPE, SIG_IGN);

    if (i == -1)
    {
        std::cout << "Error: openning conf file" << std::endl;
        return 0;
    }
    if (argc != 2)
        return (basic_error("Usage: ./parse_confile <path_to_config_file>", NULL, ""));
	std::ifstream file(argv[1]);
	if (!file.is_open())
        return (basic_error("Error: file " , argv[1] , " not found"));
	std::string line;
	std::vector<std::string> lines;
    std::cout << "BEFORE WHILE" << std::endl;
	while (std::getline(file, line))
	{
		std::replace( line.begin(), line.end(), '\t', ' ');
		// if (line.size() > 0 && line[0] != '#')
		// 	lines.push_back(line);
        size_t i = 0;
		while (i < line.size() && line[i] == ' ')
        {
            // std::cout << "i = " << i << " line[i] = [" << line[i] << "]" << std::endl;
			i++;
        }
		if (line != " " && line != "" && (i != line.size()))
			lines.push_back(line);	
	}
    file.close();
    // int set = 1;
// setsockopt(sd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
	httpServers srvrs(argc, argv, lines);
    // srvrs.set_parse_lines(lines);
	std::cout << "srvrs.httpServers_repl();" << std::endl;
	srvrs.httpServers_repl();
    std::cout << "srvrs.httpServers_repl();" << std::endl;
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
    return (0);
}
