#include "../../includes/response.hpp"


// if the hanlde method returns 1 the request will be passed to next hanle
// if the handle method returns 0 the request will be handled by the calling object
int system_block_response::handle(client cl)
{
	if (isMethodImplemented(cl.request->get_http_method()) == 0)
		return 0;
	if (isHttpVersionSupported(cl.request->get_http_version()) == 0)
		return 0;
	buildresponse(cl);
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

void request_block_response::buildresponse(client cl)
{
	switch (error)
	{
		case HTTP_VERSION_NOT_SUPPORTED_CODE:
			setResposeStatusLine(HTTP_VERSION_NOT_SUPPORTED_CODE, HTTP_VERSION_NOT_SUPPORTED_MSG);
			setResponseHeader(CONTENT_TYPE, CONTENT_TYPE_TEXT_HTML);
			setResponseBody(HTTP_VERSION_NOT_SUPPORTED_RESPONSE_MSG);
			break;
		case NOT_IMPLEMENTED_CODE:
			setResposeStatusLine(NOT_IMPLEMENTED_CODE, NOT_IMPLEMENTED_MSG);
			setResponseheader(CONTENT_TYPE, CONTENT_TYPE_TEXT_HTML);
			setResponseBody(NOT_IMPLEMENTED_RESPONSE_MSG);
			break;
		default:
			return;
	}
}
