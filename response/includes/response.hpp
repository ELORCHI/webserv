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

// building response steps
// 1. check the system and the request
	//1.1 if the system is not available, return 503
	//1.2 is the method implemented?, return 501
	//1.3 is sytem_block_ok?, return 500 enternal server error
	//1.4 is http version supported?, return 505
// 2. check the request
	//2.1 is method allowed?, return 405. doing this by checking the request method
	//2.2 is forbidden?, return. doing this by checking if the client is allowed to access the resource
	//2.3 is request_block_ok?, return 400 bad request. doing this by checking the request's syntax
// 3. check if the request should be redirected
	//doing this if the target resource is found but its a directory, return 302
	//there still more cases to be added
	//but a redirection i guess is done only after locating the resource
	// no redirection for post and delete methods
// 4. is method get?
	//4.1 is the resource found?, return 404
	//4.2 is the resource a directory without slaches?, return 302
	//4.3 is the resource a directory with slaches looking for indexes ?, return 302
		//4.3.1 if no index is found look for autoindex
			// if autoindex is found, return 200 with a list of files in the directory
			// if no autoindex is found, return 404
	//4.4 is the resource a file?, return 200
// 5. is method delete? // when this method is allowed ? the user cannot delete anything he wants? that would be a mess
	// if successful
		// return 204 accepted if th action is to be queued
		// return 204 No content if the action if the action executed but the response does not include an entity.
		// return 200 if the response includes an entity describing the status.
	// if not successfull
		// the rousource file is not found return 404
		//	Forbidden. The requester does not have permission to access the specified resource.
// 6. is method post?

// 7. 


class response //interface
{
	public:
		response();
		~response();
		virtual void handle() = 0;
};

class resonseHandler : public response// abstract class
{
	public:
		resonseHandler();
		~resonseHandler();
		virtual void handle(response *res) = 0;
		void appentTobuffer(int buffer_size, int buffer_offset, char *msg);
	private:
		char *buffer;
		int bufferSize;
		int bufferOffset;
};