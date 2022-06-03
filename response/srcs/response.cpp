#include "../../includes/response.hpp"


// if the hanlde method returns 1 the request will be passed to next hanle
// if the handle method returns 0 the request will be handled by the calling object
int system_block_response::handle(client cl)
{
	if (this->isMethodImplemented(cl.request->get_http_method()) == 0)
		return 0;
	if (this->isHttpVersionSupported(cl.request->get_http_version()) == 0)
		return 0;
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

void request_block_response::buildresponse()
{
	switch (error)
	{
		case HTTP_VERSION_NOT_SUPPORTED_CODE:
			this->setResposeStatusLine(HTTP_VERSION_NOT_SUPPORTED_CODE, HTTP_VERSION_NOT_SUPPORTED_MSG);
			this->setResponseHeader(CONTENT_TYPE, CONTENT_TYPE_TEXT_HTML);
			this->setResponseBody(HTTP_VERSION_NOT_SUPPORTED_RESPONSE_MSG);
			break;
		case NOT_IMPLEMENTED_CODE:
			this->setResposeStatusLine(NOT_IMPLEMENTED_CODE, NOT_IMPLEMENTED_MSG);
			this->setResponseheader(CONTENT_TYPE, CONTENT_TYPE_TEXT_HTML);
			this->setResponseBody(NOT_IMPLEMENTED_RESPONSE_MSG);
			break;
		default:
			return;
	}
}



// each location in the server block has its own path
// we will match the path of the request with the path of each location and chose the location with the longest match
//Nginx begins by checking all prefix-based location matches.
//It checks each location against the complete request URI.
location* locator::searchLocation(std::vector<location> locations, parse_request &req)
{
	int max_match_length = 0;
	location *loc = new location();
	int	match;
	int size = locations.size();
	for (int i = 0; i < size; i++)
	{
		match = 0;
		for (int j = 0; j < locations[i].get_path().lenght(); i++)
		{
			if (locations[i].get_path()[j] == req.get_path()[j])
				match++;
			else
				break;
		}
		if (match > max_match_length)
		{
			max_match_length = match;
			loc = locations[i];
		}
	}
	if (max_match_length == 0)
		return NULL;
	return loc;
}

void locator::setLocation(std::vector<location> locations, parse_request &req)
{
	location *loc = this->searchLocation(locations, req);
	if (loc == NULL)
		loc = this->defaultLocation();
	this->location = loc;
}

location *locator::defaultLocation()
{
	location *loc = new location();
	loc->set_path(cl.request->server_parsed_data->);
	return loc;
}