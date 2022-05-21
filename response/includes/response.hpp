#pragma once
#include "../../includes/webserv.hpp"

// #ADD DEFINES FOR STATUX CODE


// i dont know what ot do about chuncked responses

class response
{
	public:
		response(client client);
		~response();
		send();
		void setSearchLocation(server server);
		location getSearchLocation();
		char *buffer;
    private:
		int			statusCode;
		location	searchLocatin;
};


response::response(client client)
{

}

void	response::searchLocatin(server server)
{
	std::vector<location> locations = server.get_locations();
	std::vector<location>::iterator it = locations.begin();
	for (it , it < locations.end, it++)
	{
		
	}
}