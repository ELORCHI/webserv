#pragma once
#include "../../includes/webserv.hpp"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


#define GET std::string("GET")
#define POST std::string("POST")
#define DELETE std::string("DELETE")
#define HTTP_VERSION_1_1 std::string("HTTP/1.1")
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
#define BAD_REQUEST_MSG std::string("400 Bad Request")
#define OK_MSG std::string("200 OK")
#define MOVED_PERMANENTLY_MSG std::string("301 Moved Permanently")
#define NO_CONTENT_MSG std::string("204 No Content")
#define CONFLICT_MSG std::string("409 Conflict");
#define CREATED_MDG std::string("201 Created")

#define FORBIDDEN_CODE 403
#define NOT_FOUND_CODE 404
#define NOT_ALLOWED_CODE 405
#define INTERNAL_SERVER_ERROR_CODE 500
#define NOT_IMPLEMENTED_CODE 501
#define BAD_GATEWAY_CODE 502
#define SERVICE_UNAVAILABLE_CODE 503
#define HTTP_VERSION_NOT_SUPPORTED_CODE 505
#define RESPONSE_OK 200
#define RESPONSE_NOT_MODIFIED 304
#define RESPONSE_BAD_REQUEST 400
#define MOVED_PERMANENTLY 301
#define NO_CONTENT 204
#define CONFLICT 409
#define CREATED_CODE 201

#define GET_RESPONSE 0
#define POST_RESPONSE 1
#define DELETE_RESPONSE 2

#define AUTOINDEX_CODE 100

#define DIR 1
#define FI  0
#define NO -1
#define CG  2

#define NOT_FOUND_RESPONSE_MSG std::string("<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1></body></html>")
#define CONFLICT_RESPONSE_MSG std::string("<html><head><title>CONFLICT</title></head><body><h1>CONFLICT</h1></body></html>")
#define FORBIDDEN_RESPONSE_MSG std::string("<html><head><title>403 Forbidden</title></head><body><h1>403 Forbidden</h1></body></html>")
#define NOT_ALLOWED_RESPONSE_MSG std::string("<html><head><title>405 Method Not Allowed</title></head><body><h1>405 Method Not Allowed</h1></body></html>")
#define INTERNAL_SERVER_ERROR_RESPONSE_MSG std::string("<html><head><title>500 Internal Server Error</title></head><body><h1>500 Internal Server Error</h1></body></html>")
#define NOT_IMPLEMENTED_RESPONSE_MSG std::string("<html><head><title>501 Not Implemented</title></head><body><h1>501 Not Implemented</h1></body></html>")
#define BAD_GATEWAY_RESPONSE_MSG std::string("<html><head><title>502 Bad Gateway</title></head><body><h1>502 Bad Gateway</h1></body></html>")
#define SERVICE_UNAVAILABLE_RESPONSE_MSG std::string("<html><head><title>503 Service Unavailable</title></head><body><h1>503 Service Unavailable</h1></body></html>")
#define GATEWAY_TIMEOUT_RESPONSE_MSG std::string("<html><head><title>504 Gateway Timeout</title></head><body><h1>504 Gateway Timeout</h1></body></html>")
#define HTTP_VERSION_NOT_SUPPORTED_RESPONSE_MSG std::string("<html><head><title>505 HTTP Version Not Supported</title></head><body><h1>505 HTTP Version Not Supported</h1></body></html>")
#define BAD_REQUEST_RESPONSE_MSG std::string("<html><head><title>400 Bad Request</title></head><body><h1>400 Bad Request</h1></body></html>")
#define MOVED_PERMANENTLY_RESPONSE_MSG std::string("<html><head><title>Moved Permanently</title></head><body><h1>Moved Permanently</h1></body></html>")
#define NO_CONTENT_RESPONSE_MSG std::string("<html><head><title>No Content</title></head><body><h1>Moved Permanently</h1></body></html>")
#define CREATED_RESPONSE_MSG std::string("<html><head><title>Created</title></head><body><h1>CREATED</h1></body></html>")

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

// does resquest needs cgi?
	// if the request needs a cgi to be executed then the cgi will be responsblie only for the response body
	// method body without cgi will be only readu
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


// if the response is chuncked. do not add a content length header
// chunked or transfer encoding ====NEED TO READ MORE ABOUT IT====
	// if the response is chunked, the response body is sent in chunks.
	// if the response is not chunked, the response body is sent in a single chunk.
	// the chunked encoding is used when the response body is too large to be sent in a single chunk and the client is capable of receiving the response body in multiple chunks.
	// or the client specified the transfer encoding in the request.
	// send method will handle the chunked encoding and the transfer encoding




// if the hanlde method returns 1 the request will be passed to next hanle
// if the handle method returns 0 the request will be handled by the calling object

class response
{
	public:
		response();
		~response();
		virtual int handle() = 0; //the signature of amethod for handling requests
};

class responseHandler : public response// abstract class
{
	public:
		responseHandler(client cl);
		responseHandler();
		virtual ~responseHandler();
		virtual int handle() = 0;
		virtual void buildresponse() = 0;
		void appendTobuffer(int buffer_size, int buffer_offset, char *msg);// need implementation
		virtual void setResposeStatusLine(int status, std::string status_line);// need implementation
		virtual void setResponseHeaders(void);// need implementation set up general headers subclasses which need more header
		// could implement their own and they might call the parent method and then append an other headers to the response buffer
		virtual void setResponseBody(std::string body);// need implementation
	protected:
		char *buffer;
		std::string response_status_line;
		std::string	response_headers;
		std::string response_body;
		int bufferSize;// it might be a response length cause we are sending the buffer
		int bufferOffset;
		int contentLength;
		int isChunked;
		int error;
		client cl;
};


// this class checks if the system is available
// is the method implemented?
// is the http version supported?

class system_block_response : public responseHandler
{
	public:
		system_block_response();
		~system_block_response();
		int handle();
		void buildresponse();
		int isMethodImplemented(std::string method);
		int isHttpVersionSupported(std::string http_version);
	protected:
};


class workingLocation
{
	public:
		workingLocation();
		location		*searchLocation(std::vector<location> locations, std::string source);
		location		*defaultLocation(server *server);
		void			setRedirections(server *server);
		void			setDefaultError(server *server);
		void			setlocation(request *request);
		void			setUpload(std::string path);
		std::string		getUpload(void);
		std::vector<std::vector<std::string> > getRedirections(void);
		std::vector<std::vector<std::string> > getDefaultError(void);
		location		*getLocation(void);
		~workingLocation();
	private:
		location *serverlocation;
		std::string upload;
		std::vector<std::vector<std::string> >  redirections;
		std::vector<std::vector<std::string> >  defaultError;
};


// this class selects a location from the server locations to search fo the resource
class Locator : public responseHandler
{
	public:
		Locator();
		~Locator();
		int				handle();
		int				getResourceType(void);
		bool			gedEnd(void);
		bool			isMethodAllowd(std::string method);
		bool			isCgi(std::string path);
		bool			getIndex();
		bool			getAutoIndex();
		void			buildresponse();
		void			setworkingLocation(void);
		void			setResourceType(void);
		void			setResourceFullPath(void);
		void			setResourceFullPath(std::string path);
		void			checker(void);
		void			setIndex(void);
		void			setAutoIndex(void);
		std::string		getResourceFullPath(void);
		std::string		readBody(std::string path);// need implementation
		workingLocation *getWorkingLocation(void);
		std::string		getindexfile(void);
	protected:
		workingLocation *Locate;
		responseHandler *methodHandler;
		std::string		resourceFullPath;
		int				resourceType;
		bool			endwithslash;
		bool			hasIndex;
		bool			autoindex;
};

class GetHandler : public responseHandler
{
	public:
		GetHandler(Locator *_godFather);
		GetHandler(void);
		~GetHandler(void);
		int handle(void);
		int handleFiles(void);
		int HandleDir(void);
		int HandleCGI(void);
		void buildresponse(void);
		void setResponseBody(std::string body);// need implementation
		void setResponseHeaders(void);// need implementation set up general headers subclasses which need more headers
		void setGodFather(Locator *_godFather);
		std::string setAutoindexResponse(void);//needs implemantation
	private:
		Locator *godFather;
};

class DeleteHandler : public responseHandler
{
	public:
		DeleteHandler(Locator *_godFather);
		~DeleteHandler();
		int handle();
		void buildresponse();
		void setResponseBody(std::string body);// need implementation
		void setResponseHeaders(void);// need implementation set up general headers subclasses which need more headers
		void setGodFather(Locator *_godFather);
		int handleFiles(void);
		int HandleDir(void);
		int HandleCGI(void);
		int deleter(std::string path);
	private:
		Locator *godFather;
};


class PostHandler : public responseHandler
{
	public:
		PostHandler(Locator *_godFather);
		~PostHandler();
		int handle();
		void buildresponse();
		void setResponseBody(std::string body);// need implementation
		void setResponseHeaders(void);// need implementation set up general headers subclasses which need more headers
		void setGodFather(Locator *_godFather);
		int handleFiles(void);
		int HandleDir(void);
		int HandleCGI(void);
		int creator(std::string path);
		bool supportAppload();
	private:
		Locator *godFather;
};