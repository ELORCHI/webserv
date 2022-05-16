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

#define NUM_CLIENTS 10
#define PORT 1236 
#define MAX_MSG_SIZE 256
#define INVALID_SOCKET -1
#define MAX_EVENTS 1000

class server {
    int listenServerFd;
    int listenServerPort;
    struct sockaddr_in listeningServAddr;
    
    int serverKqFd;
    bool canRun;
    struct kevent _eventList[MAX_EVENTS];
    std::unordered_map<int, client*>clientmap;
    
    public:
        server(int port);
        ~server();
        bool start();
        void stop();
        void run();
        void acceptConnection();
        void disconnectClient(client *c, bool is_delete);
        void read_from_client(client *c, long data_length);
};