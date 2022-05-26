//#include "request.hpp"

// int			badRequest(request& req){
// 	req.setCode(400);
// 	std::cerr << "BAD REQUEST" << std::endl;
// 	return -1;
// }

// int toHex(std::string str){
// 	std::stringstream ss;
// 	size_t hex;

// 	ss << std::hex << str;
// 	ss >> hex;
// 	return hex;
// }

// int					request::getFirstLine(const std::string &buff, request& req)
// {
// 	std::string	line = buff.substr(0, buff.find_first_of('\n'));
// 	size_t	i, j;

// 	i = line.find_first_of(' ');
// 	if (i == std::string::npos)
// 		return badRequest(req);
// 	req._method.assign(line, 0, i);
// 	if (!req.checkMethod())
// 		return -1;
// 	if ((j = buff.find_first_not_of(' ', i)) == std::string::npos)
// 		return badRequest(req);
// 	if ((i = buff.find_first_of(' ', j)) == std::string::npos)
// 		return badRequest(req);
// 	req._path.assign(buff, j, i - j);
// 	j += i - j;
// 	if ((j = buff.find_first_not_of(' ', j)) == std::string::npos){
// 		req._retCode = 400;
// 		std::cerr << "No HTTP version" << std::endl;
// 		return -1;
// 	}
// 	if (buff[j] == 'H' && buff[j + 1] == 'T' && buff[j + 2] == 'T' &&
// 			buff[j + 3] == 'P' && buff[j + 4] == '/'){
// 		req._version.assign(buff, j + 5, 3);
// 	}
// 	if (req._version.compare("1.0") && req._version.compare("1.1")){
// 		req._retCode = 505;
// 		std::cerr << "BAD VERSION" << std::endl;
// 		return (-1);
// 	}
// 	_status = PRE_HEADERS; 
// 	return j;
// }

// int					request::ParseHeaders(std::string buff,  request& req)
// {
// 	std::string ret, key, value;
// 	size_t cursor = 0;

// 	cursor = getFirstLine(buff, req);
// 	ret = getNextLine(buff, cursor);
// 	while ((ret = getNextLine(buff, cursor)).compare("\r") && ret.compare(""))
// 	{
// 		key = removeSpace(getKey(ret));
// 		if (!key.compare("Host")){
// 			int begin = ret.find_first_of(":");
// 			begin++;
// 			std::string str = ret.substr(begin, ret.length());
// 			Host(str, req);
// 			continue;
// 		}
// 		value = removeSpace(getValue(ret, key.size()));
// 		req._headers[key] = value;
// 	}
// 	req.getQuery();
// 	if (_headers["Authorization"].compare(""))
// 		parseAuthorization(req);
// 	_status = PRE_BODY;
// 	if (!_headers["Content-Length"].compare(""))
// 		throw "Content Length not found";
// 	// print_req(req);
// 	return 1;
// }

// int			request::InternalServerError(){
// 	std::cerr << YELLOW << "InternalServerError" << RESET << std::endl;
// 	_retCode = 500;
// 	return -1;	
// }

// int					request::parseRquest(std::string buff,  request& req, int socket_fd){
// 	std::fstream _body;
// 	std::string delim("\r\n\r\n");
// 	std::string filename = "/tmp/body";
// 	size_t bodyCursor = buff.find(delim);

// 	_current_time = std::time(NULL);
// 	filename += std::to_string(socket_fd);
// 	if (bodyCursor == std::string::npos && _status == START_LINE)
// 		req._tmp += buff;
// 	else if (_status == START_LINE){
// 		req._tmp.append(buff.substr(0, bodyCursor + delim.length()));
// 		_status = HEADERS;
// 	}
// 	if (_status == HEADERS)
// 		ParseHeaders(req._tmp, req);
// 	if (_status == PRE_BODY){
// 		req._tmp.clear();
// 		req._tmp.append(buff.substr(bodyCursor + delim.length(), buff.length()));
// 		buff.clear();
// 		if (_headers["Transfer-Encoding"].compare("chunked")){
// 			_status = BODY;
// 		}
// 		else if (!_headers["Transfer-Encoding"].compare("chunked")){
// 			_status = CHUNKS;
// 			_chunkStatus = SIZE_LINE;
// 		}
// 	}
// 	if (_status == BODY){
// 		this->_tmp += buff;
// 		parseUnchunkedRequest(filename, buff);
// 	}
// 	if (_status == CHUNKS){
// 		this->_tmp += buff;
// 		parseChunkedRequest(filename);
// 	}
// 	if (_status == COMPLETE)
// 		return 0;
// 	return 1;
// }

// int request::parseUnchunkedRequest(std::string filename, std::string buff)
// {
// 	std::fstream _body;

// 	_body.open (filename, std::fstream::in | std::fstream::out | std::fstream::app);
// 	if(_body.is_open()){
// 		_bodyLength += _tmp.length();
// 		_body << _tmp.c_str();
// 	}
// 	else
// 		InternalServerError();
// 	_tmp.clear();
// 	if (_headers["Content-Length"].compare("") && _bodyLength >= stoi(_headers["Content-Length"]))
// 		_status = COMPLETE;
// 	return 0;
// }

// int request::parseChunkedRequest(std::string filename)
// {
// 	size_t end;
// 	std::fstream _body;

// 	while ((end = _tmp.find("\r\n")) != std::string::npos)
// 	{
// 		if (_chunkStatus == SIZE_LINE)
// 		{
// 			std::string hex = _tmp.substr(0, end);
// 			_chunkSize = toHex(hex);
// 			_tmp.erase(0, end + 2);
// 			_chunkStatus = CHUNK;
// 		}
// 		else if (_chunkStatus == CHUNK)
// 		{
// 			if (_chunkSize == 0){
// 				_status = COMPLETE;
// 				return 1;
// 			}
// 			_body.open (filename, std::fstream::in | std::fstream::out | std::fstream::app);
// 			if(_body.is_open()){
// 				_bodyLength += _tmp.length();
// 				_body << _tmp.substr(0, end);
// 			}
// 			else
// 				InternalServerError();
// 			_tmp.erase(0, end + 2);
// 			_chunkSize = 0;
// 			_chunkStatus = SIZE_LINE;
// 		}
// 	}
// 	return 0;
// }
// #include <iostream>
// #include <iomanip>
// #include <sstream>

// bool is_hexnumber(const std::string& str)
// {
// 	for (size_t i = 0; i < str.length(); i++)
// 	{
// 		char current = str[i];
// 		if (current >= '0' && current <= '9')
// 			continue;
// 		if (current >= 'A' && current <= 'F')
// 			continue;
// 		if (current >= 'a' && current <= 'f')
// 			continue;
// 		return false;
// 	}
// 	return true;
// }

// int main()
// {
//     int x, y;
//     std::stringstream stream;

//     if (is_hexnumber("1C"))
//         std::cout << "1"; 
//     exit(0);
//     // std::cin >> x;
//     stream << "22";
//     stream >> std::hex >> y;
// 	stream >> y;
//     std::cout << y;

//     return 0;
// }

#include <ctime>
#include <iostream>
#include <unistd.h>

std::string gen_random(const int len) {
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

int main(int argc, char *argv[]) {
    // srand((unsigned)time(NULL) * getpid());     
    std::cout << gen_random(10) << "\n";        
    return 0;
}

// 4\r\nWiki\r\n6\r\npedia \r\nE\r\nin\r\n\r\nchunks.\r\n0\r\n\r\n