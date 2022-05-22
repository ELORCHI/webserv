#include "parse_request.hpp"

parse_request::parse_request():
    _http_version(),
    _http_method(),
    _http_path(),
    _http_headers(),
    _http_body(),
	_headers_part(),
	_body_part(),
	_port_request(0)
{
}

parse_request::~parse_request()
{
}

size_t parse_request::hex_to_dec(std::string num)
{
	std::stringstream stream;
	size_t y;
	stream << num;
	stream >> std::hex >> y;
	return y;
}

bool parse_request::is_hexnumber(const std::string& str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		char current = str[i];
		if (current >= '0' && current <= '9')
			continue;
		if (current >= 'A' && current <= 'F')
			continue;
		return false;
	}
	return true;
}

bool parse_request::is_number(const std::string& str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		char current = str[i];
		if (current >= '0' && current <= '9')
			continue;
		return false;
	}
	return true;
}

void    parse_request::set_http_method(std::string &line)
{
	if (line != "POST" && line != "GET" && line != "DELETE")
	{
		std::cout << "Error: bad request" << std::endl;
		exit(0);
	}
	_http_method = line;
}

void    parse_request::set_http_version(std::string &line)
{
	if (line != "HTTP/1.1")
	{
		std::cout << "Error: bad version" << std::endl;
		exit(0);
	}
	_http_version = line;
}

void parse_request::eraseAllSubStr(std::string & mainStr, const std::string & toErase)
{
    size_t pos = std::string::npos;
    while ((pos  = mainStr.find(toErase) )!= std::string::npos)
        mainStr.erase(pos, toErase.length());
}

void	parse_request::set_chunked_http_body(std::string& lines)
{
	// std::cout << lines;
	// exit(0);
	size_t begin = 0, end;
	while ((end = lines.find("\r\n", begin)) != std::string::npos)
	{
		std::string tmp = lines.substr(begin, end - begin);
		// std::cout << tmp << std::endl;
		if (is_hexnumber(tmp))
		{
			size_t size = hex_to_dec(tmp);
			if (size == 0)
				break ;
			begin = end + 2;
			end = begin + size;
			tmp = lines.substr(begin, end - begin);
			_http_body += tmp;
			begin = end + 2;
		}
		else
		{
			std::cout << "Error: bad chunked body" << std::endl;
			exit(0);
		}
	}
}

void	parse_request::set_unchunked_http_body(std::string& lines)
{
	size_t content_length;
	if (is_number(_http_headers["Content-Length"]))
		content_length = std::stoi(_http_headers["Content-Length"]);
	else
	{
		std::cout << "Error: bad content length" << std::endl;
		exit(0);
	}
	eraseAllSubStr(lines, "\r\n");
	_http_body = lines.substr(0, content_length);
}

void	parse_request::set_http_body(std::string &line)
{
	if (_http_headers.find("Transfer-Encoding") != _http_headers.end() && _http_headers["Transfer-Encoding"] == "chunked")
		set_chunked_http_body(line);
	else if (_http_headers.find("Content-Length") != _http_headers.end())
		set_unchunked_http_body(line);
	else
	{
		std::cout << "Error: bad body" << std::endl;
		exit(0);
	}
}

void    parse_request::set_http_headers(std::string &line)
{
	size_t found;
	line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::stringstream ss(line);
	std::string token;
	std::string key;
	std::string value;
	while (std::getline(ss, token, '\n'))
	{
		found = token.find(":");
		if (found != std::string::npos)
		{
			key = token.substr(0, found);
			value = token.substr(found + 2);
			key.erase(remove(key.begin(), key.end(), ' '), key.end());
			value.erase(remove(value.begin(), value.end(), ' '), value.end());
			_http_headers[key] = value;
		}
		else
		{
			std::cout << "Error: bad headers" << std::endl;
			exit(0);
		}
	}
}

std::string    parse_request::set_http_vmp(std::string line)
{
	int i = 0;
	size_t found;
	std::string tmp, tmp2;
	found = line.find("\r\n");
	if (found != std::string::npos)
	{
		tmp = line.substr(0, found);
		tmp2 = line.substr(found + 2);
	}
    std::stringstream ss(tmp);
    std::string token;
    while (std::getline(ss, token, ' '))
    {
        if (i == 0)
			set_http_method(token);
        else if (i == 1)
        {
			size_t found;
			found = token.find('?');
			if (found != std::string::npos)
			{
				_http_path["path"].assign(token, 0, found);
				_http_path["query"].assign(token, found + 1, (token.size() - found));
			}
			else
				_http_path["path"] = token;
        }
        else if (i == 2)
			set_http_version(token);
		i++;
    }
	return (tmp2);
}

void    parse_request::set_lines(char *buffer)
{
	std::string line = buffer;
	size_t found;

	found = line.find("\r\n\r\n");
	if (found != std::string::npos)
	{
		_headers_part = line.substr(0, found);
		_body_part = line.substr(found + 4);
	}
	else
	{
		std::cout << "Error: bad request" << std::endl;
		exit(0);
	}
}

void    parse_request::start_parsing(char *buffer)
{
	std::string headers;
    this->set_lines(buffer);
    headers = this->set_http_vmp(_headers_part);
	this->set_http_headers(headers);
	set_http_body(_body_part);
	std::cout << "Method: " << _http_method << std::endl;
	std::cout << "Version: " << _http_version << std::endl;
	std::cout << "Path: " << _http_path["path"] << std::endl;
	std::cout << "Query: " << _http_path["query"] << std::endl;
	std::cout << "Fragment: " << _http_path["fragment"] << std::endl;
	std::cout << "Headers: " << std::endl;
	for(std::map <std::string, std::string>::iterator it =_http_headers.begin(); it!=_http_headers.end(); ++it)
   	{
       std::cout << it->first << " => " << it->second << '\n';
   	}
	std::cout << "body: \n"<< _http_body;
}
