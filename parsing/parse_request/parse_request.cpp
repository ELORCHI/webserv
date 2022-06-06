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
	_is_request_valid(false),
	_is_header_complete(false),
	_file_descriptor(0),
	_port_request(0),
	_my_content_length(0),
	_chunk_size(-1)
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

std::vector<std::string> parse_request::tokenize(std::string s, std::string del)
{
    std::vector<std::string> tokens;
	int start = 0;
	int end = s.find(del);
	while (1)
    {
		tokens.push_back(s.substr(start, end - start));
		start = end + del.size();
		end = s.find(del, start);
        if (end == std::string::npos)
        {
            tokens.push_back(s.substr(start, end - start));
            break;
        }
	}
	return tokens;
}

void	parse_request::set_chunked_http_body()
{
	std::vector<std::string> splited;
	splited = tokenize(_data, "\r\n");
	int i = 0;
	while (i < splited.size())
	{		
		if (_chunk_size == -1)
		{
			if (is_hexnumber(splited[i]))
				_chunk_size = hex_to_dec(splited[i]);
			else
			{
				std::cout << "Error: bad chunk size" << std::endl;
				_code_status = 505;
				return ;
			}
			if (_chunk_size == 0)
			{
				_is_request_complete = true;
				return ;
			}
		}
		else
		{
			if (splited[i].size() == 0)
			{
				write(_file_descriptor, "\r\n", 2);
				_chunk_size -= 2;
			}
			else
			{
				write(_file_descriptor, splited[i].c_str(), splited[i].size());
				_chunk_size -= splited[i].size();
			}
			// write(_file_descriptor, splited[i].c_str(), _chunk_size);
			// _chunk_size -= splited[i].size();
			if (_chunk_size == 0)
				_chunk_size = -1;
		}
		i++;
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
	write(_file_descriptor, _data.c_str(), _data.size());
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
}

void	parse_request::set_http_body()
{
	if (_http_headers.find("Transfer-Encoding") != _http_headers.end() && _http_headers.find("Content-Length") != _http_headers.end())
	{
		std::cout << "Error: bad request" << std::endl;
		_code_status = 400;
	}
	else if (_http_headers.find("Transfer-Encoding") != _http_headers.end() && _http_headers["Transfer-Encoding"] == "chunked")
		set_chunked_http_body();
	else if (_http_headers.find("Content-Length") != _http_headers.end())
		set_unchunked_http_body();
	else
	{
		std::cout << "Error: bad request" << std::endl;
		_code_status = 400;
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
	if (!_is_request_complete)
	{
		_data.append(buff, size);
		if (!_is_header_complete)
		{
			if (_data.find("\r\n\r\n") != std::string::npos)
			{
				// std::cout << _data << std::endl;
				_is_header_complete = true;
				std::string header = _data.substr(0, _data.find("\r\n\r\n"));
				header = set_http_vmp(header);
				set_http_headers(header);
				_tmp = _data.substr(_data.find("\r\n\r\n") + 4);
				_data.clear();
				std::cout << _data << std::endl;
				_tmp.clear();
				if (get_http_method() != "POST")
				{
					_is_request_complete = true;
					return _is_header_complete;
				}
			}
		}
		if (_is_header_complete && !_is_request_complete)
		{
			set_extention();
			_path_body = gen_random(10) + _extention;
			std::cout << _path_body << std::endl;
			_file_descriptor = open(_path_body.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
			if (_file_descriptor == -1)
			{
				std::cout << "Error: bad request" << std::endl;
				_code_status = 500;
				_is_request_complete = true;
				return _is_header_complete;
			}
			set_http_body();
		}
	}
	return _is_request_complete;
	// std::string headers;
    // headers = this->set_http_vmp(header);
	// if (_code_status != 0)
	// 	this->set_http_headers(headers);
	// if (_code_status != 0)
	// 	set_extention();
	// if (_code_status != 0)
	// 	set_http_body(filename);
	// if (_code_status != 0)
	// 	_code_status = 200;
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
