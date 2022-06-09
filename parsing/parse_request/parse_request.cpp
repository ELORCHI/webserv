#include "parse_request.hpp"

parse_request::parse_request():
    _http_version(),
    _http_method(),
    _http_path(),
    _http_headers(),
	_headers_part(),
	_body_part(),
	_port_request(0),
	_extention(),
	_code_status(0),
	_path_body("")
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

void	parse_request::set_chunked_http_body(std::string &filename)
{
	// extention in content_type 
	std::string line;
	size_t found, size;
	std::string tmp;
	std::ifstream fin(filename);

	_path_body = gen_random(10);
	std::ofstream fout(_path_body.append(_extention), std::ios::binary); //os::bin::ios
	if (!fout || !fin)
	{
		std::cout << "Error: file not found" << std::endl;
		_code_status = 500;
	}
	while (getline(fin, line))
	{
		if (fin.eof())
			break ;
		line += "\n";
		found = line.find("\r\n");
		if (found != std::string::npos)
		{
			tmp = line.substr(0, found);
			if (is_hexnumber(tmp))
			{
				size = hex_to_dec(tmp);
				if (size == 0)
					break;
				while (size > 0)
				{
					getline(fin, line);
					line += "\n";
					if (line.length() > size)
					{
						fout << line.substr(0, size);
						size = 0;
					}
					else if (line.length() <= size)
					{
						fout << line;
						size -= line.length();
					}
				}
			}
		}
	}
	fin.close();
	fout.close();
	// size_t begin = 0, end;
	// while ((end = lines.find("\r\n", begin)) != std::string::npos)
	// {
	// 	std::string tmp = lines.substr(begin, end - begin);
	// 	if (is_hexnumber(tmp))
	// 	{
	// 		size_t size = hex_to_dec(tmp);
	// 		if (size == 0)
	// 			break ;
	// 		begin = end + 2;
	// 		end = begin + size;
	// 		tmp = lines.substr(begin, end - begin);
	// 		_http_body += tmp;
	// 		begin = end + 2;
	// 	}
	// 	else
	// 	{
	// 		std::cout << "Error: bad chunked body" << std::endl;
	// 		exit(0);
	// 	}
	// }
}

void	parse_request::set_unchunked_http_body(std::string &filename)
{
	size_t content_length;
	std::ifstream fin(filename);

	_path_body = gen_random(10);
	std::ofstream fout(_path_body.append(_extention), std::ios::binary); //os::bin::ios
	if (!fout || !fin)
	{
		std::cout << "Error: file not found" << std::endl;
		_code_status = 500;
	}
	if (is_number(_http_headers["Content-Length"]))
		content_length = std::stoi(_http_headers["Content-Length"]);
	else
	{
		std::cout << "Error: bad content length" << std::endl;
		_code_status = 400;
	}
	// extention in content_type 
	std::string line;
	size_t found;
	std::string tmp;
	while (getline(fin, line))
	{
		if (fin.eof())
			break ;
		line += "\n";
		eraseAllSubStr(line, "\r\n");
		if (line.length() > content_length)
		{
			std::cout << "Error: bad request" << std::endl;
			_code_status = 400;
		}
		else if (line.length() <= content_length)
		{
			fout << line;
			content_length -= line.length();
		}
	}
	if (content_length != 0)
	{
		std::cout << "Error: bad request" << std::endl;
		_code_status = 411;
	}
	fin.close();
	fout.close();
	// eraseAllSubStr(lines, "\r\n");
	// _http_body = lines.substr(0, content_length);
}

void	parse_request::set_http_body(std::string &filename)
{

	if (_http_headers.find("Transfer-Encoding") != _http_headers.end() && _http_headers.find("Content-Length") != _http_headers.end())
	{
		std::cout << "Error: bad request" << std::endl;
		_code_status = 400;
	}
	else if (_http_headers.find("Transfer-Encoding") != _http_headers.end() && _http_headers["Transfer-Encoding"] == "chunked")
		set_chunked_http_body(filename);
	else if (_http_headers.find("Content-Length") != _http_headers.end())
		set_unchunked_http_body(filename);
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
		_code_status = 400;
	}
}

void    parse_request::start_parsing(std::string &header, std::string &filename)
{
	std::string headers;
    headers = this->set_http_vmp(header);
	if (_code_status != 0)
		this->set_http_headers(headers);
	if (_code_status != 0)
		set_extention();
	if (_code_status != 0)
		set_http_body(filename);
	if (_code_status != 0)
		_code_status = 200;
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
