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
		found = line.find("\r\n",found + 1);//replace "\n" by "\r\n"
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
            // _http_path = token;
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
    while (_lines.size() > i && _lines[i][1] != '\n' && _lines[i][0] != '\r')
    {
		set_http_headers(_lines[i]);
		i++;
	}
	i++;
	while (_lines.size() > i)
	{
		int chunked = 0;
		for(std::map <std::string, std::string>::iterator it=_http_headers.begin(); it!=_http_headers.end(); ++it)
		{
			if (it->first == "Transfer-Encoding" && it->second == "chunked")
			{
				chunked = 1;
				break ;
			}
		}
		this->set_http_body(_lines[i]);
		// if (chunked)
		// 	this->reset_http_body();
		i++;
	}
    std::cout << _http_method << std::endl;
    std::cout << _http_version << std::endl;
	std::cout << "fragment: " << _http_path["fragment"] << std::endl;
	std::cout << "path: " << _http_path["path"] << std::endl;
	std::cout << "query: " <<_http_path["query"] << std::endl;
	for(std::map <std::string, std::string>::iterator it=_http_headers.begin(); it!=_http_headers.end(); ++it)
   	{
       std::cout << it->first << " => " << it->second << '\n';
   	}
	std::cout << " \n body: \n"<< _http_body;
}