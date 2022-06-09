#include "parse_request.hpp"

parse_request::parse_request():
    _http_version(),
    _http_method(),
    _http_path(),
    _http_headers(),
	_path_body(""),
	_extention(),
	_code_status(0),
	_data(""),
	_is_request_complete(false),
	_is_header_complete(false),
	_file_descriptor(0),
	_port_request(0),
	_my_content_length(0),
	_chunk_size(-1),
	_is_file_created(false)
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
		_code_status = 405;
	}
	else
		_http_method = line;
}

void    parse_request::set_http_version(std::string &line)
{
	if (line != "HTTP/1.1")
	{
		std::cout << "Error: bad version" << std::endl;
		_code_status = 505;
	}
	else
		_http_version = line;
}

void parse_request::eraseAllSubStr(std::string & mainStr, const std::string & toErase)
{
    size_t pos = std::string::npos;
    while ((pos  = mainStr.find(toErase) )!= std::string::npos)
        mainStr.erase(pos, toErase.length());
}

void	parse_request::set_chunked_http_body()
{
	while (!_is_request_complete)
	{
		if (_chunk_size == -1)
		{
			size_t found = _data.find("\r\n");
			if (found != std::string::npos)
			{
				std::string chunk_size = _data.substr(0, found);
				_chunk_size = hex_to_dec(chunk_size);
				_data.erase(0, found + 2);
			}
			if (_chunk_size == 0 && (_data.size() < 3))
			{
				_is_request_complete = true;
				close(_file_descriptor);
			}
		}
		if (_chunk_size != -1)
		{
			if (_data.size() > _chunk_size + 2)
			{
				write(_file_descriptor, _data.c_str(), _chunk_size);
				_data.erase(0, _chunk_size + 2);
				_chunk_size = -1;
			}
			else
				break ;
		}
	}
}

void	parse_request::set_unchunked_http_body()
{
	size_t content_length;

	if (is_number(_http_headers["Content-Length"]))
		content_length = std::stoi(_http_headers["Content-Length"]);
	else
	{
		std::cout << "Error: bad content length" << std::endl;
		_code_status = 400;
	}
	write(_file_descriptor, _data.data(), _data.size());
	_my_content_length += _data.size();
	if (_my_content_length >= content_length)
		_is_request_complete = true;
	_data = "";
	if (_my_content_length != content_length && _is_request_complete)
	{
		std::cout << "Error: bad content length" << std::endl;
		_code_status = 400;
	}
	close(_file_descriptor);
	std::cout << _path_body << std::endl;
}

void	parse_request::set_http_body()
{
	if (_http_headers.find("Transfer-Encoding") != _http_headers.end() && _http_headers.find("Content-Length") != _http_headers.end())
	{
		std::cout << "Error: bad request" << std::endl;
		_code_status = 400;
		close(_file_descriptor);
		remove(_path_body.c_str());
		_is_request_complete = 1;
		return ;
	}
	else if (_http_headers.find("Transfer-Encoding") != _http_headers.end() && _http_headers["Transfer-Encoding"] == "chunked")
		set_chunked_http_body();
	else if (_http_headers.find("Content-Length") != _http_headers.end())
		set_unchunked_http_body();
	else
	{
		std::cout << "Error: bad request" << std::endl;
		_code_status = 400;
		close(_file_descriptor);
		remove(_path_body.c_str());
		_is_request_complete = 1;
		return ;
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
			_code_status = 500;
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
			{
				_http_path["path"] = token;
				_http_path["query"] = "";
			}
        }
        else if (i == 2)
			set_http_version(token);
		i++;
    }
	return (tmp2);
}


int    parse_request::start_parsing(char *buff, size_t size)
{
	_data.append(buff, size);
	if (!_is_request_complete)
	{
		if (_is_header_complete == false)
		{
			if (_data.find("\r\n\r\n") != std::string::npos)
			{
				std::string header = _data.substr(0, _data.find("\r\n\r\n"));
				_tmp = _data.substr(_data.find("\r\n\r\n") + 4);
				_data.clear();
				_data = _tmp;
				_tmp.clear();
				_is_header_complete = true;
				header = set_http_vmp(header);
				if (_code_status == 0)
					set_http_headers(header);
				if (get_http_method() != "POST" || _code_status)
				{
					if (!_code_status)
						_code_status = 200;
					//if !_code_status then _code_status = 200
					_is_request_complete = true;
					return _is_request_complete;
				}
			}
		}
		if (_is_header_complete && !_is_request_complete)
		{
			if (_path_body == "")
			{
				set_extention();
				_path_body = "/tmp/" + gen_random(10) + _extention;
			}
			_file_descriptor = open(_path_body.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
			if (_file_descriptor == -1)
			{
				std::cout << "Error: bad request" << std::endl;
				_code_status = 500;
				close(_file_descriptor);
				remove(_path_body.c_str());
				_is_request_complete = true;
				return _is_request_complete;
			}
			if (_code_status == 0)
				set_http_body();
		}
	}
	if (_is_request_complete && _code_status == 0)
	{
		close(_file_descriptor);
		_code_status = 200;
	}
	return _is_request_complete;
	// std::cout << "Method: " << get_http_method() << std::endl;
	// std::cout << "Version: " << get_http_version() << std::endl;
	// std::cout << "Path: " << get_http_path() << std::endl;
	// std::cout << "Query: " << get_http_query() << std::endl;
	// std::cout << "Fragment: " << _http_path["fragment"] << std::endl;
	// std::cout << "Headers: " << std::endl;
	// for(std::map <std::string, std::string>::iterator it =_http_headers.begin(); it!=_http_headers.end(); ++it)
   	// {
    //    std::cout << it->first << " => " << it->second << '\n';
   	// }
	// std::cout << "body: \n"<< get_http_body();
}

std::string parse_request::gen_random(const int len) {
	srand((unsigned)time(NULL) * getpid());     
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);
    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return tmp_s;
}
