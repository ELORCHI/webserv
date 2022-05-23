#include "../includes/response.hpp"


response::response(client cl)
{
	this->_cl = cl;
	std::string method = cl.getReadyRequest()->get_request_parsing_data().get_http_method();
	if (method == GET)
		buildGetResponse();
	else if (method == POST)
		buildPOSTRespone();
	else if (method == DELETE)
		buildDelteResponse();
	else
		buildNotAllowedResponse();
}

void response::buildGetResponse()
{
	// build the response
}

location server::getSearchLocation(std::vector<location> locations)
{
	// get the location from the request
	// return the location
	std::vector<location>::iterator it = locations.begin();
	while (it != locations.end())
	{
		
			it++;
	}

}