#include "clients.h"
#include <iostream>

client::client(int fd, struct sockaddr_in addr)
{
    clientFd = fd;
    clientAddr = addr;
    is_done_reading_from = false;
    read_buffer.resize(8000);
}


void client::appendToReadBuffer(char *buffer)
{
    read_buffer += buffer;
    if (read_buffer.find("\n\n") != std::string::npos)
    {
        std::cout << "header found" << std::endl;
    }
}


client::~client() {}