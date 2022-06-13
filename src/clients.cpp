
   
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
	// isHeadersBufferResidue = false;
	this->keepAliveData.timeout = 0;
	this->keepAliveData.max = 0;
	this->keepAliveData.is_keepAlive = false;
	this->keepAliveData.is_connection = true;
	this->keepAliveData.connection_type = "close";
	this->response_buffer.resize(8000);
	this->response_buffer = "";
	this->send_offset = 0;
	this->is_done_sending_to = false;
	// this->resp = NULL;
	//bodyFileName = std::string("/tmp/") +  getRandomName();
	//bodyFile.open(bodyFileName.c_str(), std::ios::out | std::ios::binary | std::ios::app);
}

client& client::operator= (const client& other)
{
	clientFd = other.clientFd;
	clientAddr = other.clientAddr;
	is_done_reading_from = other.is_done_reading_from;
	read_buffer = other.read_buffer;
	ready_request = other.ready_request;
	// isHeadersBufferResidue = other.isHeadersBufferResidue;
	this->keepAliveData.timeout = other.keepAliveData.timeout;
	this->keepAliveData.max = other.keepAliveData.max;
	this->keepAliveData.is_keepAlive = other.keepAliveData.is_keepAlive;
	this->keepAliveData.is_connection = other.keepAliveData.is_connection;
	this->keepAliveData.connection_type = other.keepAliveData.connection_type;
	this->response_buffer = other.response_buffer;
	this->send_offset = other.send_offset;
	this->is_done_sending_to = other.is_done_sending_to;
	// this->resp = NULL;
	//bodyFileName = other.bodyFileName;
	//bodyFile.open(bodyFileName.c_str(), std::ios::out | std::ios::binary | std::ios::app);
	return *this;	
}

//copy constructor for client using deep copy of the request object
client::client(const client& other)
{
	clientFd = other.clientFd;
	clientAddr = other.clientAddr;
	is_done_reading_from = other.is_done_reading_from;
	read_buffer = other.read_buffer;
	ready_request = new request(this->pr, other.getReadyRequest()->get_server());
	// isHeadersBufferResidue = other.isHeadersBufferResidue;
	this->keepAliveData.timeout = other.keepAliveData.timeout;
	this->keepAliveData.max = other.keepAliveData.max;
	this->keepAliveData.is_keepAlive = other.keepAliveData.is_keepAlive;
	this->keepAliveData.is_connection = other.keepAliveData.is_connection;
	this->keepAliveData.connection_type = other.keepAliveData.connection_type;
	this->response_buffer = other.response_buffer;
	this->send_offset = other.send_offset;
	this->is_done_sending_to = other.is_done_sending_to;
	// this->resp = NULL;
	//bodyFileName = other.bodyFileName;
	//bodyFile.open(bodyFileName.c_str(), std::ios::out | std::ios::binary | std::ios::app);
} 




client::~client() {
	if (ready_request)
		delete ready_request;
	
}

 request *client::getReadyRequest() const
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

void client::setKeepAliveInfo(std::string _kad)
{
	//get first number from _kad
	std::string s = _kad;
	std::string timeout = s.substr(s.find("timeout=") + 8, s.find(",") - s.find("timeout=") - 8);
    std::string max = s.substr(s.find("max=") + 4);
	std::stringstream ss;
	ss << timeout;
	ss >> this->keepAliveData.timeout;
	ss.clear();
	ss << max;
	ss >> this->keepAliveData.max;
	this->keepAliveData.is_keepAlive = true;
}

void client::setConnectionType(std::string connectionType)
{
	this->keepAliveData.is_connection = true;
	this->keepAliveData.connection_type = connectionType;
}
