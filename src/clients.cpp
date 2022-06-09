
   
#include "clients.h"
#include <iostream>
#include "request.hpp"


std::string getRandomName()
{
	char value[12];	
	std::ifstream devrandom("/dev/random");
	devrandom.read(value, 12);
	unsigned int random = *(unsigned int*)value;
	std::stringstream ss;
	ss << random;
	devrandom.close();
	return ss.str();
}

client::client(int fd, struct sockaddr_in addr)
{
    clientFd = fd;
    clientAddr = addr;
    is_done_reading_from = false;
    read_buffer.resize(8000);
	ready_request = NULL;
	isHeadersBufferResidue = false;
	bodyFileName = std::string("/tmp/") +  getRandomName();
	bodyFile.open(bodyFileName.c_str(), std::ios::out | std::ios::binary);
}


void client::appendToHeadersBuffer(char *buffer)
{
    this->headersBuffer += buffer; 
}

void client::appendToReadBodyFile(const char *buffer)
{
	bodyFile << buffer;
}


client::~client() {
	if (ready_request)
		delete ready_request;
	
}

request *client::getReadyRequest()
{
	return (this->ready_request);
}


bool client::isThereARequestReady()
{
	if (this->ready_request)
		return true;
	return false;
}

void client::setRequest(request *rq)
{
	this->ready_request = rq;
}

