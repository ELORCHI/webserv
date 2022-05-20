#pragma once

<<<<<<< HEAD
class response
{
    public:
            //response constructor takes a request and servers vector
            //identify which server to handle the request
            //generate a response object <still ambiguous> needs some steps to do
    private:

=======
#include "../../includes/webserv.hpp"
class response
{
	public:
		response();
		response(const std::vector<server>& Servers, const parse_request& request);
		~response();
		const server& serverIdentify(const std::vector<server>& Servers, const parse_request& request);
		const location& locationIdentify(const server& Server);
    private:
>>>>>>> ecae89fa4657b24587d23e55f248f03ab5d3669a
};