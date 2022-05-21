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
	std::size_t found = 0;
	int k = 0;
	int i = 0;
	while (1)
	{
		found = line.find("\r\n", found + 2);//replace "\n" by "\r\n"
		if (found == std::string::npos)
		{
			std::string tmp = line.substr(k, line.size());
			// tmp.assign(line, k , line.size() - k);
			_lines.push_back(tmp);
			break ;
		}
		else if (found != std::string::npos)
		{
			std::string tmp;
			tmp.assign(line, k ,(found - k));
			if (k == found)
			{
				if (i == 0)
				{
					// i = 1;
					_lines.push_back("\n");
				}
				else
				{
					k = found + 2;
					continue;
				}
			}
			else
			{
				_lines.push_back(tmp);
			}
			k = found + 2;
		}
	}
	// i = 0;
	// while (_lines.size() > i)
	// {
	// 	std::cout << _lines[i] << std::endl;
	// 	i++;
	// }
	// exit(0);
}

// HTTP/1.1
void    parse_request::set_http_method(std::string &line)
{
	if (line != "POST" && line != "GET" && line != "DELETE")
		throw std::runtime_error("Error: BAD METHOD");
	_http_method = token;
}

void    parse_request::set_http_version(std::string &line)
{
	if (line != "HTTP/1.1")
		throw std::runtime_error("Error: BAD VERSION");
	_http_version = token;
}

void    parse_request::set_http_vmp(std::string line)
{
    int i = 0;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ' '))
    {
        if (i == 0)
			set_method(token);
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
			set_version(token);
        i++;
    }
}

void    parse_request::set_http_headers(std::string line)
{
	int i = 0;
	std::stringstream ss(line);
	std::string token;
	std::string key;
	std::string value;
	if (line.find(":") == std::string::npos)
		throw std::runtime_error("Error: BAD REQUEST");
	while (std::getline(ss, token, ':'))
	{
		if (i == 0)
			key = token;
		else if (i == 1)
		{
			value = token;
			value.erase(remove_if(value.begin(), value.end(), isspace), value.end());
		}
		if (key == "Host" && i == 2)
		{
			value += ":";
			value += token;
		}
		i++;
	}
	_http_headers[key] = value;
}

void	parse_request::set_http_body(std::vector<std::string> line, int i)
{
	if (_http_headers.find("Transfer-Encoding") != _http_headers.end() && _http_headers["Transfer-Encoding"] == "chunked")
		set_chunked_http_body(line, i);
	else
		set_unchunked_http_body(line, i);
	// while (_lines.size() > i)
	// {
	// 	std::cout << _lines[i] << std::endl;
	// 	i++;
	// }
	// exit (0);
}

void    parse_request::start_parsing(char *buffer)
{
    int i = 1;
    this->set_lines(buffer);
    this->set_http_vmp(_lines[0]);
    while (_lines.size() > i && _lines[i][0] != '\n')
    {
		this->set_http_headers(_lines[i]);
		i++;
	}
	i++;
	set_http_body(_lines, i);
    std::cout << _http_method << std::endl;
    std::cout << _http_version << std::endl;
	std::cout << "fragment: " << _http_path["fragment"] << std::endl;
	std::cout << "path: " << _http_path["path"] << std::endl;
	std::cout << "query: " <<_http_path["query"] << std::endl;
	for(std::map <std::string, std::string>::iterator it =_http_headers.begin(); it!=_http_headers.end(); ++it)
   	{
       std::cout << it->first << " => " << it->second << '\n';
   	}
	std::cout << " \n body: \n"<< _http_body;
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