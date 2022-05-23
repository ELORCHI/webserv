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
	
}