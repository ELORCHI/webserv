#pragma once
#include "../../includes/webserv.hpp"


#define GET std::string("GET")
#define POST std::string("POST")
#define DELETE std::string("DELETE")

class response
{
	private:
		char *buffer;
		int	 status;
		int isFinished;
		client _cl;
	public:
		response(client cl);
		void buildGetResponse();
		void buildPOSTRespone();
		void buildDelteResponse();
		void buildNotAllowedResponse();
		unsigned int send();
		~response();
};




