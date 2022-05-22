#pragma once
#include <netinet/in.h>
#include <vector>
#include <string>
#include <queue>
#include "../parsing/parse_request/parse_request.hpp"
#include "request.hpp"



class client {
    int clientFd;
    struct sockaddr_in clientAddr;
    bool is_done_reading_from;
    std::string read_buffer;
	request* ready_request;
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
        request *getReadyRequest();
        bool isRequestsQueueEmpty();
        bool isThereARequestReady();
		void setRequest(request *rq);

};