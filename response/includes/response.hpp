#pragma once
#include "../../includes/webserv.hpp"


class searchLocatin
{

};

class response
{
	public:
		response(client client);
		~response();
		searchLocation searchLocatin;
		send();
    private:
		int status;
};


response::response(client client)
{
	
}