#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
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

#define NUM_CLIENTS 10
#define PORT 1236 
#define MAX_EVENTS 32
#define MAX_MSG_SIZE 256

class server {
    int listentingServerFd;
    int listeningServerPort;
    int listeningServerAddress
    int serverKqueueFd;
    void 

};

#endif