#include "../../includes/response.hpp"


// if the hanlde method returns 1 the request will be passed to next hanle
// if the handle method returns 0 the request will be handled by the calling object
int system_block_response::handle(client cl)
{
	if (isMethodImplemented(cl.re) == 0)
	{
		return 0;
	}
	return 0;
}

int system_block_response::isMethodImplemented(std::string method)
{
	if (method == GET || method == POST || method == DELETE)
		return 1;
	else
		return 0;
}

int system_block_response::isHttpVersionSupported(std::string http_version)
{
	if (http_version == HTTP_VERSION_1_1)
		return 1;
	else
		return 0;
}