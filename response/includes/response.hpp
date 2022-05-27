#pragma once
#include "../../includes/webserv.hpp"
#include <stdlib.h>
#include <string.h>


#define GET std::string("GET")
#define POST std::string("POST")
#define DELETE std::string("DELETE")

#define HTTP_VERSION_MSG std::string("HTTP/1.1")
#define FORBIDDEN_MSG std::string("403 Forbidden")
#define NOT_FOUND_MSG std::string("404 Not Found")
#define NOT_ALLOWED_MSG std::string("405 Method Not Allowed")
#define INTERNAL_SERVER_ERROR_MSG std::string("500 Internal Server Error")
#define NOT_IMPLEMENTED_MSG std::string("501 Not Implemented")
#define BAD_GATEWAY_MSG std::string("502 Bad Gateway")
#define SERVICE_UNAVAILABLE_MSG std::string("503 Service Unavailable")
#define GATEWAY_TIMEOUT_MSG std::string("504 Gateway Timeout")
#define HTTP_VERSION_NOT_SUPPORTED_MSG std::string("505 HTTP Version Not Supported")
#define HTTP_VERSION_NOT_SUPPORTED_BY_SERVER_MSG std::string("505 HTTP Version Not Supported by Server")

#define FORBIDDEN_CODE 403
#define NOT_FOUND_CODE 404
#define NOT_ALLOWED_CODE 405
#define INTERNAL_SERVER_ERROR_CODE 500
#define NOT_IMPLEMENTED_CODE 501
#define BAD_GATEWAY_CODE 502
#define SERVICE_UNAVAILABLE_CODE 503
#define HTTP_VERSION_NOT_SUPPORTED_BY_SERVER_CODE 505
#define RESPONSE_OK 200
#define RESPONSE_NOT_MODIFIED 304
#define RESPONSE_BAD_REQUEST 400

#define GET_RESPONSE 0
#define POST_RESPONSE 1
#define DELETE_RESPONSE 2


class response 
{
	protected:
		int status;
		char *body;
		char *buffer;
		int bufferSize;
		int bufferOffset;
		int responseType;
		int bodySize;
		client *client;
	public:
		response();
		~response();
		void setBufferOffset(int buffer_offset);
		void setResponseType(int responseType);
		void setBuffer(char *buffer, int buffer_size);
		void setBodySize(int bodySize);
		char *getBuffer(void);
		char *getBody(void);
		int getStatus(void);
		int getBufferSize(void);
		int getBufferOffset(void);
		int getResponseType(void);
		int getBodySize(void);
		virtual void setStatus(int status) = 0;
		virtual void setBody(char *body) = 0;
};

class systemResponse : public response
{
	public:
		systemResponse();
		~systemResponse();
		void setBody(char *body);
};


class requestErrorResponse : public response
{
	public:
		requestErrorResponse();
		~requestErrorResponse();
		void setStatus(int status);
		void setBody(char *body);
};

class acceptedRequestResponse : public response
{
	public:
		acceptedRequestResponse();
		~acceptedRequestResponse();
		void setStatus(int status);
		void setBody(char *body);
};

class getResponse : public acceptedRequestResponse
{
	public:
		getResponse();
		~getResponse();
		void setStatus(int status);
		void setBody(char *body);
};

class postResponse : public acceptedRequestResponse
{
	public:
		postResponse();
		~postResponse();
		void setStatus(int status);
		void setBody(char *body);
};

class deleteResponse : public acceptedRequestResponse
{
	public:
		deleteResponse();
		~deleteResponse();
		void setStatus(int status);
		void setBody(char *body);
};
