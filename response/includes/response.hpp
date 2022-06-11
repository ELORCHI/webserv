#pragma once
#include "../../includes/webserv.hpp"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <cctype>


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
#define CONFLICT_MSG std::string("409 Conflict")
#define CREATED_MSG std::string("201 Created")

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

#define DIRE 4
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
		responseHandler(client *_cl);
		responseHandler();
		virtual ~responseHandler();
		virtual int handle() = 0;
		virtual void buildresponse() = 0;
		virtual void setClient(client *_cl);
		virtual client *getClient(void);
		// std::string setexten(std::string path);
		std::string getBuffer(void);
	protected:
		std::string buffer;
		std::string response_body;
		std::string statusLine;
		std::string headers;
		int error;
		client *cl;
};


// this class checks if the system is available
// is the method implemented?
// is the http version supported?

class system_block_response : public responseHandler
{
	public:
		system_block_response(client *cl);
		~system_block_response();
		int handle();
		void buildresponse();
		int isMethodImplemented(std::string method);
		int isHttpVersionSupported(std::string http_version);
		void setClient(client *cl);
		client *getClient(void);
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
		void			setCgi(cgi cgi);
		workingLocation(const workingLocation &other);
		std::string		getUpload(void);
		std::vector<std::vector<std::string> >	getRedirections(void);
		std::vector<std::vector<std::string> >	getDefaultError(void);
		std::string								getDefaultError(int erroCode);
		location		*getLocation(void);
		cgi				*getCgi(void);
		~workingLocation();
	private:
		location *serverlocation;//free
		std::string upload;
		std::vector<std::vector<std::string> >  redirections;
		std::vector<std::vector<std::string> >  defaultError;
		cgi *cgiLocation;// free
};


// this class selects a location from the server locations to search fo the resource
class Locator : public responseHandler
{
	public:
		Locator();
		Locator(client *_cl);
		~Locator();
		int				handle();
		int				getResourceType(void);
		int				isredirection(void);
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
		void			setClient(client *_cl);
		std::string		getResourceFullPath(void);
		std::string		readBody(std::string path);
		workingLocation *getWorkingLocation(void);
		std::string		getindexfile(void);
		std::string		getContentType(void);

		int				HandleCGI();
	protected:
		workingLocation *Locate;//free
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
		void buildresponse(void);
		void setClient(client *_cl);
		void setGodFather(Locator *_godFather);
		std::string setAutoindexResponse(void);
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
		void setClient(client *_cl);
		void setGodFather(Locator *_godFather);
		int handleFiles(void);
		int HandleDir(void);
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
		void setGodFather(Locator *_godFather);
		int handleFiles(void);
		int HandleDir(void);
		int creator(std::string path);
		bool supportAppload();
		void setClient(client *_cl);
		client *getClient(void);
	private:
		Locator *godFather;
};




std::string getResponseStatusLine(int status, std::string status_line);

bool isError(int status);




//thsi will handle get post delete and locating problem
// this will use a function to get default error pages
// this function cant handle system_response object
// system_response_object will need only get default error pages function
std::string getDefaultError(int status, Locator *info);
std::string getResponseBody(int status, std::string bodyMsg, Locator *info);
std::string    formatted_time(void);
std::string getResponseHeaders(int status, Locator *info, int body_size);
std::string readBody(std::string path);

responseHandler *getResponse(client  *cl);
std::string getErroBody(int erroCode, std::string definebody, client *cl);

