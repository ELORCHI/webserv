#pragma once

#include "httpServer.hpp"

class httpServers {
    std::unordered_map<int, socket_data> sharedPortsSockets;
    std::vector<httpServer> _servers;
    std::vector<int> getRepeatedPorts(std::vector<server> parsed_servers_data);
    public:
    httpServers(int argc, char **argv);
    ~httpServers() {}
    void httpServers_repl();
};