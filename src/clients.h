#pragma once
#include <netinet/in.h>
class client {
    int clientFd;
    struct sockaddr_in clientAddr;

    public:
        client(int fd, struct sockaddr_in addr);
        ~client();

        sockaddr_in getClientAddr()
        {
            return clientAddr;
        }

        int getClientFd()
        {
            return clientFd;
        }

};