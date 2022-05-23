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

// based on an http decision table we will decide wich type of response we will send
// by type of response i mean: first if there is a problem with the request we will send a response to
// so if we have a bad request we will send a response with a bad request code
// if we have a not found request we will send a response with a not found code
// if we have a not allowed request we will send a response with a not allowed code
// if we have a not implemented request we will send a response with a not implemented code
// if we have a bad gateway request we will send a response with a bad gateway code
// anyways if a request passes all the tests we will send a response with a 200 code
// each time a response passes a test we will move to the next one in the decision table
// until we reach the end of the decision table
// which will tell us how to build our response



// the responseType Decider will implement the decision table
// the responseTypeDecider will return the type of response we will send
// we may call the responseTypeDecider  in the response constructor
// if not i suggest that we  implement a function called buildHttpResponse
// which will call the responseTypeDecider and return the type of response we will send
// then we construct a response object with a client object and decison type
// but before this build httpresponse will set some useful data to the response object
// such as searchlocation cgi parameters and so on
// why do is this?
// because searchlocation and other variables need to be set in the procces of responseTypeDecider


int responseTypeDecider(client cl)
{
	std::string method = cl.getReadyRequest()->get_request_parsing_data().get_http_method();
	if (method == GET)
		return GET_RESPONSE;
	else if (method == POST)
		return POST_RESPONSE;
	else if (method == DELETE)
		return DELETE_RESPONSE;
	else
		return NOT_ALLOWED_RESPONSE;
}

response buildHttpResponse(client cl)
{
	response res(cl);

	res.setResponseType()
}

