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
	while (1)
	{
		found = line.find("\n",found + 1);//replace "\n" by "\r\n"
		if (found == std::string::npos)
		{
			std::string tmp;// = line.substr(k, line.size());
			tmp.assign(line, k , line.size() - k);
			_lines.push_back(tmp);
			break;
		}
		else if (found!=std::string::npos)
		{
			std::string tmp;
			// std::cout << "found: " << found << std::endl;
			// std::cout << "k: " << k << std::endl;
			// std::cout << "*******" << std::endl;
			if (k == found)
				_lines.push_back("\n");
			else
			{
				tmp.assign(line, k ,(found - k));
				_lines.push_back(tmp);
			}
			k = found + 1;
		}
	}
}

void    parse_request::set_http_vmp(std::string line)
{
    int i = 0;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ' '))
    {
        if (i == 0)
        {
            _http_method = token;
        }
        else if (i == 1)
        {
            _http_path = token;
        }
        else if (i == 2)
        {
            _http_version = token;
        }
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
	while (std::getline(ss, token, ':'))
	{
		if (i == 0)
			key = token;
		else if (i == 1)
			value = token;
		if (key == "Host" && i == 2)
		{
			value += ":";
			value += token;
		}
		i++;
	}
	_http_headers[key] = value;
}

void	parse_request::set_http_body(std::string line)
{
	_http_body += line;
	_http_body += "\n";
}

void    parse_request::start_parsing(char *buffer)
{
    int i = 1;
    this->set_lines(buffer);
    this->set_http_vmp(_lines[0]);
    while (_lines.size() > i && _lines[i][0] != '\n')
    {
		set_http_headers(_lines[i]);
		i++;
	}
	i++;
	while (_lines.size() > i)
	{
		this->set_http_body(_lines[i]);
		i++;
	}
    std::cout << _http_method << std::endl;
    std::cout << _http_path << std::endl;
    std::cout << _http_version << std::endl;
	for(std::map <std::string, std::string>::iterator it=_http_headers.begin(); it!=_http_headers.end(); ++it)
   	{
       std::cout << it->first << " => " << it->second << '\n';
   	}
	std::cout << " \n body: \n"<< _http_body;
}