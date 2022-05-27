#pragma once
#include "../../includes/webserv.hpp"


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
	private:
		int status;
		char *buffer;
		int buffer_size;
		int buffer_offset;
		int responseType;
	public:
		response();
		~response();
		void set_status(int status);
		int get_status();
		void set_buffer(char *buffer, int buffer_size);
		char *get_buffer();
		int get_buffer_size();
		int get_buffer_offset();
		void set_buffer_offset(int buffer_offset);
		void set_responseType(int responseType);
		int get_responseType();
		
};