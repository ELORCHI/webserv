#pragma once
#include <iostream>
#include <sys/_types/_size_t.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <exception>
#include "error_exception.hpp"
#include <fcntl.h>
#include "clients.h"
#include <unordered_map>
#include <algorithm>
#include "../parsing/parse_confile/parse_confile.hpp"
#include "../parsing/parse_request/parse_request.hpp"
#include <unordered_set>
#include <set>


#define NUM_CLIENTS 10
#define PORT 6000
#define MAX_MSG_SIZE 256
#define INVALID_SOCKET -1
#define MAX_EVENTS 1000
class parse_config;

typedef struct {
    int listenServerFd;
    int listenServerPort;
    struct sockaddr_in listeningServAddr; 
} socket_data;

class httpServer
{
    int listenServerFd;
    int listenServerPort;
    struct sockaddr_in listeningServAddr; 
    int serverKqFd;
    bool canRun;
    struct kevent _eventList[MAX_EVENTS];
    std::unordered_map<int, client*>clientmap;
    bool is_shared_port;
    server server_parsed_data;
    public:
        // httpServer(int port);
        httpServer(server server_parsed_data,  bool is_shared_port, socket_data *sd);
        ~httpServer() {}
        bool start();
        void stop();
        void run();
        void acceptConnection();
        void disconnectClient(client *c, bool is_delete);
        void read_from_client(client *c, long data_length);
    //handeling multiple servers interface
    public:
        static std::vector<httpServer> servers;
        static std::set<int> getRepeatedPorts(std::vector<server> parsed_servers_data);
        static socket_data *create_listening_socket(int port);
        static int	parsing_conf(int ac, char **av,parse_config *conf);
};