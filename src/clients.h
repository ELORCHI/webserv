#pragma once
#include <netinet/in.h>
#include <vector>
#include <string>
#include <queue>
#include "../parsing/parse_request/parse_request.hpp"
#include "request.hpp"
#include <fstream>
#include <sstream>


class client {
    int clientFd;
    struct sockaddr_in clientAddr;
    bool is_done_reading_from;
    std::string read_buffer;

	bool isHeadersReadingComplete;
	std::fstream readTmpFile;
	std::string headersBuffer;
	bool isHeadersBufferResidue;
	std::string bodyFileName;
	std::fstream bodyFile;
	request* ready_request;
    parse_request pr;
    client& operator= (const client& other);
    typedef struct {
        int timeout;
        int max;
        bool is_keepAlive;
        std::string connection_type;
        bool is_connection; 
    } keepAlive;
    keepAlive keepAliveData;
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
        void appendToHeadersBuffer(char *buffer);
		void appendToReadBodyFile(const char *buffer, size_t size);
		std::fstream &getBodyFile() { return bodyFile; }
        std::string getHeadersBuffer() {return headersBuffer;}
        request *getReadyRequest();
        bool isRequestsQueueEmpty();
        bool isThereARequestReady();
		void setRequest(request *rq);
		std::fstream &getReadTmpFile();
		bool is_request_complete(long long recieved_data_size, char *buffer);
        parse_request &get_pr() { return pr; }
        keepAlive getKeepAliveInfo() { return this->keepAliveData; };
        void setKeepAliveInfo(std::string _kad);
        void setConnectionType(std::string connectionType);
};