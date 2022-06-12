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
#include "../src/request.hpp"
#include <unordered_set>
#include <set>
#include <fstream>
#include "../response/includes/response.hpp"
//#include "servers.hpp"


#define NUM_CLIENTS 10
#define PORT 6000
#define MAX_MSG_SIZE 256
#define INVALID_SOCKET -1
#define MAX_EVENTS 8000
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
   // struct kevent _eventList[MAX_EVENTS];
    std::map<int, client*>clientmap;
    bool is_shared_port;
    server server_parsed_data;
	int run_once;
    public:
        // httpServer(int port);
        httpServer(server server_parsed_data,  bool is_shared_port, socket_data *sd, int KqueueFd);
        ~httpServer() {}
        bool start();
        void stop();
        void run(int num_events, struct kevent *eventList);
        void acceptConnection();
        void disconnectClient(client *c, bool is_delete);
        void read_from_client(client *c, long data_length);
        void write_to_client(client *cl, long data_length, std::string response_buffer);                         
        bool doesHttpRequestBelongs(request *rq);
        int getServerFd();
        server *getServerParsedData() { 
            server *s = new server;
            *s = server_parsed_data;
            return s; 
        };
    //handeling multiple servers interface
    public:
        static std::vector<httpServer> servers;
        static std::set<int> getRepeatedPorts(std::vector<server> parsed_servers_data);
        static socket_data *create_listening_socket(int port, std::string host);
        static int	parsing_conf(int ac, char **av,parse_config *conf);
};
