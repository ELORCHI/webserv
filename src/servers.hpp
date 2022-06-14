#pragma once

#include "httpServer.hpp"
#include <map>

class httpServers {
    std::map<int, socket_data> sharedPortsSockets;
    std::vector<httpServer> _servers;
    std::vector<int> getRepeatedPorts(std::vector<server> parsed_servers_data);
    int KqueueFd;
    std::vector<std::string> _lines;
    struct kevent _eventList[MAX_EVENTS];
    public:
    httpServers(int argc, char **argv, std::vector<std::string> lines);
    ~httpServers() {}
    void acceptConnection();
    std::vector<std::string> get_parse_lines() const
    {
        return _lines;
    }
    void disconnectClient(client *c, bool is_delete);
    void httpServers_repl();
};