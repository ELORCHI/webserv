#pragma once

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
};