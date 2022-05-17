#pragma once
#include <netinet/in.h>
#include <vector>
#include <string>

class client {
    int clientFd;
    struct sockaddr_in clientAddr;
    bool is_done_reading_from;
    std::string read_buffer;
    
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

        bool is_reading_complete() { return is_done_reading_from; }
        void set_reading_status (bool status) { is_done_reading_from = status; }
        void appendToReadBuffer(char *buffer); 
        std::string getReadBuffer() {return read_buffer;}
};