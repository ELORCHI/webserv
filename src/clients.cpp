#include "clients.h"
#include <iostream>
#include "request.hpp"

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
    if (request::is_request_complete(read_buffer))
        is_done_reading_from = true;
}


client::~client() {}