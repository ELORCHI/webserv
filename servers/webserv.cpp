#include "ServersSocketsClass.hpp"

int webserv(char *ConfFile)
{
    std::vector<server> ServersConf; // this vector Hold all servers configuration parsed. see parsing/server.hpp for details
    std::vector<Servers> servers;
    // starting by creating server
    IniteServer(servers, ServersConf);
}
