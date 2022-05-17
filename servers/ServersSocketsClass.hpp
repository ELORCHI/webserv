#pragma once

#include "../includes/webserv.hpp"

//this class inherit from yhebbat class to have acces to servers config
// and add some subclasses for managing requests and preparing for responses


class Servers : public server
{
    private:

    public:
};

int webserv(char *ConfFile);