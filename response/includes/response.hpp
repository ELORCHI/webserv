#pragma once
#include "../../includes/webserv.hpp"


class response
{
	public:
		response(client client);
		~response();
		send();
		location getSearchLocation(server server);
		char *buffer;
    private:
		int status;
		location searchLocatin;
};


response::response(client client)
{

}