#include "parse_request.hpp"

parse_request::parse_request():
    _lines(),
    _http_version(),
    _http_method(),
    _http_path(),
    _http_headers(),
    _http_body()
{
}

parse_request::~parse_request()
{
}

void    parse_request::set_lines(char *buffer)
{
	std::string line = buffer;
	size_t found;

	found = line.find("\r\n");
	if (found != std::string::npos)
	{
		_http_headers = line.substr(0, found);
		_http_body = line.substr(found + 2);
	}
	else
	{
		std::cout << "Error: bad request" << std::endl;
		exit(0);
	}
}

void    parse_request::set_http_method(std::string &line)
{
	if (line != "POST" && line != "GET" && line != "DELETE")
	{
		std::cout << "Error: bad request" << std::endl;
		exit(0);
	}
	_http_method = token;
}

void    parse_request::set_http_version(std::string &line)
{
	if (line != "HTTP/1.1")
	{
		std::cout << "Error: bad version" << std::endl;
		exit(0);
	}
	_http_version = token;
}

std::string    parse_request::set_http_vmp(std::string line)
{
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
			size_t found1;
			found = token.find('?');
			found1 = token.find('#');
			if (found != std::string::npos && found1 != std::string::npos)
			{
				_http_path["path"].assign(token, 0, found);
				_http_path["query"].assign(token, found + 1, (found1 - found - 1));
				_http_path["fragment"].assign(token, found1 + 1, (token.size() - found1));
			}
			else if (found != std::string::npos && found1 == std::string::npos)
			{
				_http_path["path"].assign(token, 0, found);
				_http_path["query"].assign(token, found + 1, (token.size() - found));
			}
			else if (found == std::string::npos && found1 != std::string::npos)
			{
				_http_path["path"].assign(token, 0, found1);
				_http_path["fragment"].assign(token, found1 + 1, (token.size() - (found1 + 1)));
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

void    parse_request::set_http_headers(std::string &line)
{
	size_t found;
	line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::stringstream ss(line);
	std::string token;
	std::string key;
	std::string value;
	// if (line.find(":") == std::string::npos)
	// 	throw std::runtime_error("Error: BAD REQUEST");
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
	_http_headers[key] = value;
}

// void	parse_request::set_http_body(std::vector<std::string> line, int i)
// {
// 	if (_http_headers.find("Transfer-Encoding") != _http_headers.end() && _http_headers["Transfer-Encoding"] == "chunked")
// 		set_chunked_http_body(line, i);
// 	else
// 		set_unchunked_http_body(line, i);
// 	// while (_lines.size() > i)
// 	// {
// 	// 	std::cout << _lines[i] << std::endl;
// 	// 	i++;
// 	// }
// 	// exit (0);
// }

void    parse_request::start_parsing(char *buffer)
{
	std::string headers;
    this->set_lines(buffer);
    headers = this->set_http_vmp(_http_headers);
	this->set_http_headers(headers);
	// set_http_body(_lines, i);
	// std::cout << " \n body: \n"<< _http_body;
}

size_t parse_request::hex_to_dec(std::string num)
{
	std::stringstream stream;
	size_t y;
	stream << num;
	stream >> std::hex >> y;
	return y;
}

bool parse_request::is_number(const std::string& str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		char current = str[i];
		if (current >= '0' && current <= '9')
			continue;
		if (current >= 'A' && current <= 'F')
			continue;
		if (current >= 'a' && current <= 'f')
			continue;
		return false;
	}
	return true;
}

void	parse_request::set_chunked_http_body(std::vector<std::string> lines, int i)
{
	size_t chunk_size;
	
	while (lines.size() > i)
	{
		if (is_number(lines[i]) && lines[i] != "0")
		{
			chunk_size = hex_to_dec(lines[i]);
			_http_body += lines[i + 1].substr(0, chunk_size);
		}
		if (lines[i] == "\n")
			_http_body += "\n";
		i++;
	}
}

void	parse_request::set_unchunked_http_body(std::vector<std::string> lines, int i)
{
	while (_lines.size() > i)
	{
		_http_body += lines[i];
		i++;
	}
	_http_body += "\n";
}