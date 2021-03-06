#include "parse_request.hpp"

parse_request::parse_request():
	_is_request_complete(false),
	_is_file_created(false),
	_is_header_complete(false),
	_code_status(0),
	_chunk_size(-1),
	_data(""),
    _http_version(),
    _http_method(),
	_path_body(""),
	_extention(),
	_file_descriptor(0),
	_port_request(0),
    _http_path(),
    _http_headers(),
	_my_content_length(0),
	_my_body_size(0)
{
}

parse_request::parse_request(parse_request const& rh)
{

	
		_http_version = rh._http_version;
		_http_method = rh._http_method;
		_http_path = rh._http_path;
		_http_headers = rh._http_headers;
		_path_body = rh._path_body;
		_extention = rh._extention;
		_code_status = rh._code_status;
		_data = rh._data;
		_is_request_complete = rh._is_request_complete;
		_is_header_complete = rh._is_header_complete;
		_file_descriptor = rh._file_descriptor;
		_port_request = rh._port_request;
		_my_content_length = rh._my_content_length;
		_chunk_size = rh._chunk_size;
		_is_file_created = rh._is_file_created;
	
}

//copy constructor
parse_request::parse_request(parse_request & rh)
{
	//std::cout << "copy constructor" << std::endl;
	_http_version = rh._http_version;
	_http_method = rh._http_method;
	_http_path = rh._http_path;
	_http_headers = rh._http_headers;
	_path_body = rh._path_body;
	_extention = rh._extention;
	_code_status = rh._code_status;
	_data = rh._data;
	_is_request_complete = rh._is_request_complete;
	_is_header_complete = rh._is_header_complete;
	_file_descriptor = rh._file_descriptor;
	_port_request = rh._port_request;
	_my_content_length = rh._my_content_length;
	_chunk_size = rh._chunk_size;
	_is_file_created = rh._is_file_created;
	_my_body_size = rh._my_body_size;
}

parse_request                       &parse_request::operator=(parse_request const & rhs)
{
	// if (rhs != *this)
	// {
		_http_version = rhs._http_version;
		_http_method = rhs._http_method;
		_http_path = rhs._http_path;
		_http_headers = rhs._http_headers;
		_path_body = rhs._path_body;
		_extention = rhs._extention;
		_code_status = rhs._code_status;
		_data = rhs._data;
		_is_request_complete = rhs._is_request_complete;
		_is_header_complete = rhs._is_header_complete;
		_file_descriptor = rhs._file_descriptor;
		_port_request = rhs._port_request;
		_my_content_length = rhs._my_content_length;
		_chunk_size = rhs._chunk_size;
		_is_file_created = rhs._is_file_created;
		_my_body_size = rhs._my_body_size;
		return *this;
	// }
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
			if ((int)_data.size() > _chunk_size + 2)
			{
				int t = write(_file_descriptor, _data.c_str(), _chunk_size);
				if (t == -1)
				{
					_is_request_complete = true;
					close(_file_descriptor);
					_code_status = 500;
				}
				_my_body_size += _chunk_size;
				_data.erase(0, _chunk_size + 2);
				_chunk_size = -1;
				if (t == 0)
					continue;
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
		_is_request_complete = true;
		close(_file_descriptor);
		return ;
	}
	int t = write(_file_descriptor, _data.data(), _data.size());
	if (t == -1)
	{
		_is_request_complete = true;
		close(_file_descriptor);
		_code_status = 500;
		return ;
	}
	_my_content_length += _data.size();
	_my_body_size = _my_content_length;
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
	// write(2, buff, size);
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
					// std::cout << "Path: " << get_http_path() << std::endl;

					if (!_code_status)
						_code_status = 200;
					_my_body_size = 0;
					//if !_code_status then _code_status = 200
					_is_request_complete = true;
					//std::cout << "****************************************body_size: " << get_body_size() << std::endl;
					return _is_request_complete;
				}
			}
		}
		if (_is_header_complete && !_is_request_complete)
		{
			if (_path_body == "")
			{
				set_extention();
				_path_body = gen_random(12) + _extention;
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
	// if (_is_request_complete)
	return _is_request_complete;
		//std::cout << "****************************************body_size: " << get_body_size() << std::endl;
	// std::cout << "Path: " << get_http_path() << std::endl;
	// std::cout << "Method: " << get_http_method() << std::endl;
	// std::cout << "Version: " << get_http_version() << std::endl;
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