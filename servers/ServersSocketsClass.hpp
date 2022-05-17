#pragma once

#include "../includes/webserv.hpp"
#include <fcntl.h>
//this class inherit from yhebbat class to have acces to servers config
// and add some subclasses for managing requests and preparing for responses

#include <netinet/in.h>
# define ALLOWED_QUEUE 5

class SocketData
{
    private:
        struct sockaddr_in socketaddress;
        int    socketfd;
    public:
        int setSocketFd(void);
        int setSocketaddress(int port);
        int bindSocket(void) const;
        int listenSocket(void) const;
};

class Servers : public server
{
    private:
        SocketData Socket;
    public:
        Servers();
        ~Servers();
        class ServerCreation : public std::exception
        {

        };
        int setSocket(void);
};

int webserv(char *ConfFile);
void IniteServers(std::vector<Servers> &servers, std::vector<server> ServersConf);

int Servers::setSocket(void)
{
    if (Socket.setSocketFd() || Socket.setSocketaddress(atoi(_listen_port.c_str())))
        return (1);
    if (Socket.bindSocket())
    {
        return (1);
    }
    if (Socket.listenSocket())
        return (1);
    return (0);
}

int SocketData::setSocketFd()
{
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1)
        return (1);
    fcntl(socketfd, F_SETFL, O_NONBLOCK);
    return (0);
}

int SocketData::setSocketaddress(int port)
{
    memset((char*)&socketaddress, 0, sizeof(socketaddress));
    socketaddress.sin_family = AF_INET;
    socketaddress.sin_addr.s_addr = htonl(INADDR_ANY);
    socketaddress.sin_port = htons(port);
}

int SocketData::bindSocket(void) const
{
    int yes = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if (bind(socketfd, (struct sockaddr *)&socketaddress, sizeof(socketaddress)) == -1)
    {
        return (1);
    }
}

int SocketData::listenSocket(void) const
{
    if (listen(socketfd, ALLOWED_QUEUE) == -1)
        return (1);
    return (0);
}