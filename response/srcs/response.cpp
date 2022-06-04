#include "../includes/response.hpp"


// if the hanlde method returns 1 the request will be passed to next handler
// if the handle method returns 0 the request will be handled by the calling object
int system_block_response::handle()
{
	int err = 0;
	if (this->isMethodImplemented(cl.getReadyRequest()->get_request_parsing_data().get_http_method()) == 1)
		err = 1;
	else if (this->isHttpVersionSupported(cl.getReadyRequest()->get_request_parsing_data().get_http_version()) == 1)
		err = 1;
	else if (cl.getReadyRequest()->get_request_parsing_data().get_code_status() == 500)
	{
		err = 1;
		error = INTERNAL_SERVER_ERROR_CODE;
	}
	if (err = 1)
		this->buildresponse(cl);
	return 1;
}

int system_block_response::isMethodImplemented(std::string method)
{
	if (method == GET || method == POST || method == DELETE)
		return 0;
	error = NOT_IMPLEMENTED_CODE;
	return 1;
}

int system_block_response::isHttpVersionSupported(std::string http_version)
{
	if (http_version == HTTP_VERSION_1_1)
		return 0;
	error = HTTP_VERSION_NOT_SUPPORTED_CODE;
	return 1;
}

void system_block_response::buildresponse(client &cl)
{
	switch (error)
	{
		case HTTP_VERSION_NOT_SUPPORTED_CODE:
			this->setResposeStatusLine(HTTP_VERSION_NOT_SUPPORTED_CODE, HTTP_VERSION_NOT_SUPPORTED_MSG);
			responseHandler::setResponseHeaders();//no header so just default ones
			this->setResponseBody(HTTP_VERSION_NOT_SUPPORTED_RESPONSE_MSG);
			break;
		case NOT_IMPLEMENTED_CODE:
			this->setResposeStatusLine(NOT_IMPLEMENTED_CODE, NOT_IMPLEMENTED_MSG);
			responseHandler::setResponseHeaders();// no header just default ones
			this->setResponseBody(NOT_IMPLEMENTED_RESPONSE_MSG);
		case INTERNAL_SERVER_ERROR_CODE:
			this->setResposeStatusLine(INTERNAL_SERVER_ERROR_CODE, INTERNAL_SERVER_ERROR_MSG);
			responseHandler::setResponseHeaders();//no header so just default ones
			this->setResponseBody(INTERNAL_SERVER_ERROR_RESPONSE_MSG);
			break;
		default:
			return;
	}
}



// each location in the server block has its own path
// we will match the path of the request with the path of each location and chose the location with the longest match
//Nginx begins by checking all prefix-based location matches.
//It checks each location against the complete request URI.
location* Locator::searchLocation(std::vector<location> &locations, std::string source)
{
	int max_match_length = 0;
	location *loc = new location();
	int	match;
	int size = locations.size();
	for (int i = 0; i < size; i++)
	{
		match = 0;
		for (int j = 0; j < locations[i].get_locations_path().size(); i++)
		{
			if (locations[i].get_locations_path()[j] == source[j])
				match++;
			else
				break;
		}
		if (match > max_match_length)
		{
			max_match_length = match;
			*loc = locations[i];
		}
	}
	if (max_match_length == 0)
		return (NULL);
	return loc;
}

void Locator::setLocation(void)
{
	location *loc = this->searchLocation(cl.getReadyRequest()->get_server()->get_location(), cl.getReadyRequest()->get_request_parsing_data().get_http_path());
	if (loc == NULL)
		loc = this->defaultLocation(cl.getReadyRequest()->get_server());
	this->workingLocation = loc;
}


// talk with yassine about this
// NEEDS MORE DETAILS UPLOAD PATH ECT MAYBE ADD CGI TO A LOCATION
location *Locator::defaultLocation(server *server)
{
	location *loc = new location();
	loc->set_root(server->get_root());
	loc->set_client_max_body_size(server->get_client_max_body_size());
	loc->set_index(server->get_index());
	loc->set_autoindex(server->get_autoindex());
	loc->fill_allowed_methods(server->get_allowed_methods());
	return loc;
}

// sets isAllowed to true if found in the location methods
// needs getters for allowed methods
// void Locator::setMethodAllowance(std::string method)
// {
// 	for (int i; i < workingLocation->get_allowd_methods().size; i++)
// 	{

// 	}
// }

int Locator::handle()
{
	
}