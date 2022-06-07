#pragma once

#include "httpServer.hpp"
#include <map>

class httpServers {
    std::map<int, socket_data> sharedPortsSockets;
    std::vector<httpServer> _servers;
    std::vector<int> getRepeatedPorts(std::vector<server> parsed_servers_data);
    int KqueueFd;
    struct kevent _eventList[MAX_EVENTS];
    public:
    httpServers(int argc, char **argv);
    ~httpServers() {}
    void acceptConnection();
    void disconnectClient(client *c, bool is_delete);
    void httpServers_repl();
};