#pragma once

#include "../includes/webserv.hpp"

//this class inherit from yhebbat class to have acces to servers config
// and add some subclasses for managing requests and preparing for responses

#include <netinet/in.h>

class socket
{
    private:
        struct sockaddr_in socketaddress;
        int    socketfd;
    public:

};

class Servers : public server
{
    private:

    public:
        Servers();
        ~Servers();
        class ServerCreation : public std::exception
        {

        };
};

int webserv(char *ConfFile);
void IniteServers(std::vector<Servers> &servers, std::vector<server> ServersConf);