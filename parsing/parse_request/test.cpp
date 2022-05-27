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

// std::string gen_random(const int len) {
// 	srand((unsigned)time(NULL) * getpid());     
//     static const char alphanum[] =
//         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//         "abcdefghijklmnopqrstuvwxyz";
//     std::string tmp_s;
//     tmp_s.reserve(len);
//     for (int i = 0; i < len; ++i) {
//         tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
//     }
//     return tmp_s;
// }

// int main(int argc, char *argv[]) {
//     // srand((unsigned)time(NULL) * getpid());     
//     std::cout << gen_random(10) << "\n";        
//     return 0;
// }
int main()
{
    std::vector <std::pair <std::string, std::string> > extentions;
    extentions.push_back(std::make_pair(".*3gpp", "audio/3gpp"));
    extentions.push_back(std::make_pair(".*jpm", "video/jpm"));
    extentions.push_back(std::make_pair(".*mp3", "audio/mp3"));
    extentions.push_back(std::make_pair(".*rtf", "text/rtf"));
    extentions.push_back(std::make_pair(".*wav", "audio/wave"));
    extentions.push_back(std::make_pair(".*xml", "text/xml"));
    extentions.push_back(std::make_pair(".3g2", "video/3gpp2"));
    extentions.push_back(std::make_pair(".3gp", "video/3gpp"));
    extentions.push_back(std::make_pair(".3gpp", "video/3gpp"));
    extentions.push_back(std::make_pair(".ac", "application/pkix-attr-cert"));
    extentions.push_back(std::make_pair(".adp", "audio/adpcm"));
    extentions.push_back(std::make_pair(".ai", "application/postscript"));
    extentions.push_back(std::make_pair(".apng", "image/apng"));
    extentions.push_back(std::make_pair(".appcache", "text/cache-manifest"));
    extentions.push_back(std::make_pair(".asc", "application/pgp-signature"));
    extentions.push_back(std::make_pair(".atom", "application/atom+xml"));
    extentions.push_back(std::make_pair(".atomcat", "application/atomcat+xml"));
    extentions.push_back(std::make_pair(".atomsvc", "application/atomsvc+xml"));
    extentions.push_back(std::make_pair(".au", "audio/basic"));
    extentions.push_back(std::make_pair(".aw", "application/applixware"));
    extentions.push_back(std::make_pair(".bdoc", "application/bdoc"));
    extentions.push_back(std::make_pair(".bin", "application/octet-stream"));
    extentions.push_back(std::make_pair(".bmp", "image/bmp"));
    extentions.push_back(std::make_pair(".bpk", "application/octet-stream"));
    extentions.push_back(std::make_pair(".buffer", "application/octet-stream"));
    extentions.push_back(std::make_pair(".ccxml", "application/ccxml+xml"));
    extentions.push_back(std::make_pair(".cdmia", "application/cdmi-capability"));
    extentions.push_back(std::make_pair(".cdmic", "application/cdmi-container"));
    extentions.push_back(std::make_pair(".cdmid", "application/cdmi-domain"));
    extentions.push_back(std::make_pair(".cdmio", "application/cdmi-object"));
    extentions.push_back(std::make_pair(".cdmiq", "application/cdmi-queue"));
    extentions.push_back(std::make_pair(".cer", "application/pkix-cert"));
    extentions.push_back(std::make_pair(".cgm", "image/cgm"));
    extentions.push_back(std::make_pair(".class", "application/java-vm"));
    extentions.push_back(std::make_pair(".coffee", "text/coffeescript"));
    extentions.push_back(std::make_pair(".conf", "text/plain"));
    extentions.push_back(std::make_pair(".cpt", "application/mac-compactpro"));
    extentions.push_back(std::make_pair(".crl", "application/pkix-crl"));
    extentions.push_back(std::make_pair(".css", "text/css"));
    extentions.push_back(std::make_pair(".csv", "text/csv"));
    extentions.push_back(std::make_pair(".cu", "application/cu-seeme"));
    extentions.push_back(std::make_pair(".davmount", "application/davmount+xml"));
    extentions.push_back(std::make_pair(".dbk", "application/docbook+xml"));
    extentions.push_back(std::make_pair(".deb", "application/octet-stream"));
    extentions.push_back(std::make_pair(".def", "text/plain"));
    extentions.push_back(std::make_pair(".deploy", "application/octet-stream"));
    extentions.push_back(std::make_pair(".disposition-notification", "message/disposition-notification"));
    extentions.push_back(std::make_pair(".dist", "application/octet-stream"));
    extentions.push_back(std::make_pair(".distz", "application/octet-stream"));
    extentions.push_back(std::make_pair(".dll", "application/octet-stream"));
    extentions.push_back(std::make_pair(".dmg", "application/octet-stream"));
    extentions.push_back(std::make_pair(".dms", "application/octet-stream"));
    extentions.push_back(std::make_pair(".doc", "application/msword"));
    extentions.push_back(std::make_pair(".dot", "application/msword"));
    extentions.push_back(std::make_pair(".drle", "image/dicom-rle"));
    extentions.push_back(std::make_pair(".dssc", "application/dssc+der"));
    extentions.push_back(std::make_pair(".dtd", "application/xml-dtd"));
    extentions.push_back(std::make_pair(".dump", "application/octet-stream"));
    extentions.push_back(std::make_pair(".ear", "application/java-archive"));
    extentions.push_back(std::make_pair(".ecma", "application/ecmascript"));
    extentions.push_back(std::make_pair(".elc", "application/octet-stream"));
    extentions.push_back(std::make_pair(".emf", "image/emf"));
    extentions.push_back(std::make_pair(".eml", "message/rfc822"));
    extentions.push_back(std::make_pair(".emma", "application/emma+xml"));
    extentions.push_back(std::make_pair(".eps", "application/postscript"));
    extentions.push_back(std::make_pair(".epub", "application/epub+zip"));
    extentions.push_back(std::make_pair(".es", "application/ecmascript"));
    extentions.push_back(std::make_pair(".exe", "application/octet-stream"));
    extentions.push_back(std::make_pair(".exi", "application/exi"));
    extentions.push_back(std::make_pair(".exr", "image/aces"));
    extentions.push_back(std::make_pair(".ez", "application/andrew-inset"));
    extentions.push_back(std::make_pair(".fits", "image/fits"));
    extentions.push_back(std::make_pair(".g3", "image/g3fax"));
    extentions.push_back(std::make_pair(".gbr", "application/rpki-ghostbusters"));
    extentions.push_back(std::make_pair(".geojson", "application/geo+json"));
    extentions.push_back(std::make_pair(".gif", "image/gif"));
    extentions.push_back(std::make_pair(".glb", "model/gltf-binary"));
    extentions.push_back(std::make_pair(".gltf", "model/gltf+json"));
    extentions.push_back(std::make_pair(".gml", "application/gml+xml"));
    extentions.push_back(std::make_pair(".gpx", "application/gpx+xml"));
    extentions.push_back(std::make_pair(".gram", "application/srgs"));
    extentions.push_back(std::make_pair(".grxml", "application/srgs+xml"));
    extentions.push_back(std::make_pair(".gxf", "application/gxf"));
    extentions.push_back(std::make_pair(".gz", "application/gzip"));
    extentions.push_back(std::make_pair(".h261", "video/h261"));
    extentions.push_back(std::make_pair(".h263", "video/h263"));
    extentions.push_back(std::make_pair(".h264", "video/h264"));
    extentions.push_back(std::make_pair(".heic", "image/heic"));
    extentions.push_back(std::make_pair(".heics", "image/heic-sequence"));
    extentions.push_back(std::make_pair(".heif", "image/heif"));
    extentions.push_back(std::make_pair(".heifs", "image/heif-sequence"));
    extentions.push_back(std::make_pair(".hjson", "application/hjson"));
    extentions.push_back(std::make_pair(".hlp", "application/winhlp"));
    extentions.push_back(std::make_pair(".hqx", "application/mac-binhex40"));
    extentions.push_back(std::make_pair(".htm", "text/html"));
    extentions.push_back(std::make_pair(".html", "text/html"));
    extentions.push_back(std::make_pair(".ics", "text/calendar"));
    extentions.push_back(std::make_pair(".ief", "image/ief"));
    extentions.push_back(std::make_pair(".ifb", "text/calendar"));
    extentions.push_back(std::make_pair(".iges", "model/iges"));
    extentions.push_back(std::make_pair(".igs", "model/iges"));
    extentions.push_back(std::make_pair(".img", "application/octet-stream"));
    extentions.push_back(std::make_pair(".in", "text/plain"));
    extentions.push_back(std::make_pair(".ini", "text/plain"));
    extentions.push_back(std::make_pair(".ink", "application/inkml+xml"));
    extentions.push_back(std::make_pair(".inkml", "application/inkml+xml"));
    extentions.push_back(std::make_pair(".ipfix", "application/ipfix"));
    extentions.push_back(std::make_pair(".iso", "application/octet-stream"));
    extentions.push_back(std::make_pair(".jade", "text/jade"));
    extentions.push_back(std::make_pair(".jar", "application/java-archive"));
    extentions.push_back(std::make_pair(".jls", "image/jls"));
    extentions.push_back(std::make_pair(".jp2", "image/jp2"));
    extentions.push_back(std::make_pair(".jpe", "image/jpeg"));
    extentions.push_back(std::make_pair(".jpeg", "image/jpeg"));
    extentions.push_back(std::make_pair(".jpf", "image/jpx"));
    extentions.push_back(std::make_pair(".jpg", "image/jpeg"));
    extentions.push_back(std::make_pair(".jpg2", "image/jp2"));
    extentions.push_back(std::make_pair(".jpgm", "video/jpm"));
    extentions.push_back(std::make_pair(".jpgv", "video/jpeg"));
    extentions.push_back(std::make_pair(".jpm", "image/jpm"));
    extentions.push_back(std::make_pair(".jpx", "image/jpx"));
    extentions.push_back(std::make_pair(".js", "application/javascript"));
    extentions.push_back(std::make_pair(".json", "application/json"));
    extentions.push_back(std::make_pair(".json5", "application/json5"));
    extentions.push_back(std::make_pair(".jsonld", "application/ld+json"));
    extentions.push_back(std::make_pair(".jsonml", "application/jsonml+json"));
    extentions.push_back(std::make_pair(".jsx", "text/jsx"));
    extentions.push_back(std::make_pair(".kar", "audio/midi"));
    extentions.push_back(std::make_pair(".ktx", "image/ktx"));
    extentions.push_back(std::make_pair(".less", "text/less"));
    extentions.push_back(std::make_pair(".list", "text/plain"));
    extentions.push_back(std::make_pair(".litcoffee", "text/coffeescript"));
    extentions.push_back(std::make_pair(".log", "text/plain"));
    extentions.push_back(std::make_pair(".lostxml", "application/lost+xml"));
    extentions.push_back(std::make_pair(".lrf", "application/octet-stream"));
    extentions.push_back(std::make_pair(".m1v", "video/mpeg"));
    extentions.push_back(std::make_pair(".m21", "application/mp21"));
    extentions.push_back(std::make_pair(".m2a", "audio/mpeg"));
    extentions.push_back(std::make_pair(".m2v", "video/mpeg"));
    extentions.push_back(std::make_pair(".m3a", "audio/mpeg"));
    extentions.push_back(std::make_pair(".m4a", "audio/mp4"));
    extentions.push_back(std::make_pair(".m4p", "application/mp4"));
    extentions.push_back(std::make_pair(".ma", "application/mathematica"));
    extentions.push_back(std::make_pair(".mads", "application/mads+xml"));
    extentions.push_back(std::make_pair(".man", "text/troff"));
    extentions.push_back(std::make_pair(".manifest", "text/cache-manifest"));
    extentions.push_back(std::make_pair(".map", "application/json"));
    extentions.push_back(std::make_pair(".mar", "application/octet-stream"));
    extentions.push_back(std::make_pair(".markdown", "text/markdown"));
    extentions.push_back(std::make_pair(".mathml", "application/mathml+xml"));
    extentions.push_back(std::make_pair(".mb", "application/mathematica"));
    extentions.push_back(std::make_pair(".mbox", "application/mbox"));
    extentions.push_back(std::make_pair(".md", "text/markdown"));
    extentions.push_back(std::make_pair(".me", "text/troff"));
    extentions.push_back(std::make_pair(".mesh", "model/mesh"));
    extentions.push_back(std::make_pair(".meta4", "application/metalink4+xml"));
    extentions.push_back(std::make_pair(".metalink", "application/metalink+xml"));
    extentions.push_back(std::make_pair(".mets", "application/mets+xml"));
    extentions.push_back(std::make_pair(".mft", "application/rpki-manifest"));
    extentions.push_back(std::make_pair(".mid", "audio/midi"));
    extentions.push_back(std::make_pair(".midi", "audio/midi"));
    extentions.push_back(std::make_pair(".mime", "message/rfc822"));
    extentions.push_back(std::make_pair(".mj2", "video/mj2"));
    extentions.push_back(std::make_pair(".mjp2", "video/mj2"));
    extentions.push_back(std::make_pair(".mjs", "application/javascript"));
    extentions.push_back(std::make_pair(".mml", "text/mathml"));
    extentions.push_back(std::make_pair(".mods", "application/mods+xml"));
    extentions.push_back(std::make_pair(".mov", "video/quicktime"));
    extentions.push_back(std::make_pair(".mp2", "audio/mpeg"));
    extentions.push_back(std::make_pair(".mp21", "application/mp21"));
    extentions.push_back(std::make_pair(".mp2a", "audio/mpeg"));
    extentions.push_back(std::make_pair(".mp3", "audio/mpeg"));
    extentions.push_back(std::make_pair(".mp4", "video/mp4"));
    extentions.push_back(std::make_pair(".mp4a", "audio/mp4"));
    extentions.push_back(std::make_pair(".mp4s", "application/mp4"));
    extentions.push_back(std::make_pair(".mp4v", "video/mp4"));
    extentions.push_back(std::make_pair(".mpd", "application/dash+xml"));
    extentions.push_back(std::make_pair(".mpe", "video/mpeg"));
    extentions.push_back(std::make_pair(".mpeg", "video/mpeg"));
    extentions.push_back(std::make_pair(".mpg", "video/mpeg"));
    extentions.push_back(std::make_pair(".mpg4", "video/mp4"));
    extentions.push_back(std::make_pair(".mpga", "audio/mpeg"));
    extentions.push_back(std::make_pair(".mrc", "application/marc"));
    extentions.push_back(std::make_pair(".mrcx", "application/marcxml+xml"));
    extentions.push_back(std::make_pair(".ms", "text/troff"));
    extentions.push_back(std::make_pair(".mscml", "application/mediaservercontrol+xml"));
    extentions.push_back(std::make_pair(".msh", "model/mesh"));
    extentions.push_back(std::make_pair(".msi", "application/octet-stream"));
    extentions.push_back(std::make_pair(".msm", "application/octet-stream"));
    extentions.push_back(std::make_pair(".msp", "application/octet-stream"));
    extentions.push_back(std::make_pair(".mxf", "application/mxf"));
    extentions.push_back(std::make_pair(".mxml", "application/xv+xml"));
    extentions.push_back(std::make_pair(".n3", "text/n3"));
    extentions.push_back(std::make_pair(".nb", "application/mathematica"));
    extentions.push_back(std::make_pair(".oda", "application/oda"));
    extentions.push_back(std::make_pair(".oga", "audio/ogg"));
    extentions.push_back(std::make_pair(".ogg", "audio/ogg"));
    extentions.push_back(std::make_pair(".ogv", "video/ogg"));
    extentions.push_back(std::make_pair(".ogx", "application/ogg"));
    extentions.push_back(std::make_pair(".omdoc", "application/omdoc+xml"));
    extentions.push_back(std::make_pair(".onepkg", "application/onenote"));
    extentions.push_back(std::make_pair(".onetmp", "application/onenote"));
    extentions.push_back(std::make_pair(".onetoc", "application/onenote"));
    extentions.push_back(std::make_pair(".onetoc2", "application/onenote"));
    extentions.push_back(std::make_pair(".opf", "application/oebps-package+xml"));
    extentions.push_back(std::make_pair(".otf", "font/otf"));
    extentions.push_back(std::make_pair(".owl", "application/rdf+xml"));
    extentions.push_back(std::make_pair(".oxps", "application/oxps"));
    extentions.push_back(std::make_pair(".p10", "application/pkcs10"));
    extentions.push_back(std::make_pair(".p7c", "application/pkcs7-mime"));
    extentions.push_back(std::make_pair(".p7m", "application/pkcs7-mime"));
    extentions.push_back(std::make_pair(".p7s", "application/pkcs7-signature"));
    extentions.push_back(std::make_pair(".p8", "application/pkcs8"));
    extentions.push_back(std::make_pair(".pdf", "application/pdf"));
    extentions.push_back(std::make_pair(".pfr", "application/font-tdpfr"));
    extentions.push_back(std::make_pair(".pgp", "application/pgp-encrypted"));
    extentions.push_back(std::make_pair(".pkg", "application/octet-stream"));
    extentions.push_back(std::make_pair(".pki", "application/pkixcmp"));
    extentions.push_back(std::make_pair(".pkipath", "application/pkix-pkipath"));
    extentions.push_back(std::make_pair(".pls", "application/pls+xml"));
    extentions.push_back(std::make_pair(".png", "image/png"));
    extentions.push_back(std::make_pair(".prf", "application/pics-rules"));
    extentions.push_back(std::make_pair(".ps", "application/postscript"));
    extentions.push_back(std::make_pair(".pskcxml", "application/pskc+xml"));
    extentions.push_back(std::make_pair(".qt", "video/quicktime"));
    extentions.push_back(std::make_pair(".raml", "application/raml+yaml"));
    extentions.push_back(std::make_pair(".rdf", "application/rdf+xml"));
    extentions.push_back(std::make_pair(".rif", "application/reginfo+xml"));
    extentions.push_back(std::make_pair(".rl", "application/resource-lists+xml"));
    extentions.push_back(std::make_pair(".rld", "application/resource-lists-diff+xml"));
    extentions.push_back(std::make_pair(".rmi", "audio/midi"));
    extentions.push_back(std::make_pair(".rnc", "application/relax-ng-compact-syntax"));
    extentions.push_back(std::make_pair(".rng", "application/xml"));
    extentions.push_back(std::make_pair(".roa", "application/rpki-roa"));
    extentions.push_back(std::make_pair(".roff", "text/troff"));
    extentions.push_back(std::make_pair(".rq", "application/sparql-query"));
    extentions.push_back(std::make_pair(".rs", "application/rls-services+xml"));
    extentions.push_back(std::make_pair(".rsd", "application/rsd+xml"));
    extentions.push_back(std::make_pair(".rss", "application/rss+xml"));
    extentions.push_back(std::make_pair(".rtf", "application/rtf"));
    extentions.push_back(std::make_pair(".rtx", "text/richtext"));
    extentions.push_back(std::make_pair(".s3m", "audio/s3m"));
    extentions.push_back(std::make_pair(".sbml", "application/sbml+xml"));
    extentions.push_back(std::make_pair(".scq", "application/scvp-cv-request"));
    extentions.push_back(std::make_pair(".scs", "application/scvp-cv-response"));
    extentions.push_back(std::make_pair(".sdp", "application/sdp"));
    extentions.push_back(std::make_pair(".ser", "application/java-serialized-object"));
    extentions.push_back(std::make_pair(".setpay", "application/set-payment-initiation"));
    extentions.push_back(std::make_pair(".setreg", "application/set-registration-initiation"));
    extentions.push_back(std::make_pair(".sgi", "image/sgi"));
    extentions.push_back(std::make_pair(".sgm", "text/sgml"));
    extentions.push_back(std::make_pair(".sgml", "text/sgml"));
    extentions.push_back(std::make_pair(".shex", "text/shex"));
    extentions.push_back(std::make_pair(".shf", "application/shf+xml"));
    extentions.push_back(std::make_pair(".shtml", "text/html"));
    extentions.push_back(std::make_pair(".sig", "application/pgp-signature"));
    extentions.push_back(std::make_pair(".sil", "audio/silk"));
    extentions.push_back(std::make_pair(".silo", "model/mesh"));
    extentions.push_back(std::make_pair(".slim", "text/slim"));
    extentions.push_back(std::make_pair(".slm", "text/slim"));
    extentions.push_back(std::make_pair(".smi", "application/smil+xml"));
    extentions.push_back(std::make_pair(".smil", "application/smil+xml"));
    extentions.push_back(std::make_pair(".snd", "audio/basic"));
    extentions.push_back(std::make_pair(".so", "application/octet-stream"));
    extentions.push_back(std::make_pair(".spp", "application/scvp-vp-response"));
    extentions.push_back(std::make_pair(".spq", "application/scvp-vp-request"));
    extentions.push_back(std::make_pair(".spx", "audio/ogg"));
    extentions.push_back(std::make_pair(".sru", "application/sru+xml"));
    extentions.push_back(std::make_pair(".srx", "application/sparql-results+xml"));
    extentions.push_back(std::make_pair(".ssdl", "application/ssdl+xml"));
    extentions.push_back(std::make_pair(".ssml", "application/ssml+xml"));
    extentions.push_back(std::make_pair(".stk", "application/hyperstudio"));
    extentions.push_back(std::make_pair(".styl", "text/stylus"));
    extentions.push_back(std::make_pair(".stylus", "text/stylus"));
    extentions.push_back(std::make_pair(".svg", "image/svg+xml"));
    extentions.push_back(std::make_pair(".svgz", "image/svg+xml"));
    extentions.push_back(std::make_pair(".t", "text/troff"));
    extentions.push_back(std::make_pair(".t38", "image/t38"));
    extentions.push_back(std::make_pair(".tei", "application/tei+xml"));
    extentions.push_back(std::make_pair(".teicorpus", "application/tei+xml"));
    extentions.push_back(std::make_pair(".text", "text/plain"));
    extentions.push_back(std::make_pair(".tfi", "application/thraud+xml"));
    extentions.push_back(std::make_pair(".tfx", "image/tiff-fx"));
    extentions.push_back(std::make_pair(".tif", "image/tiff"));
    extentions.push_back(std::make_pair(".tiff", "image/tiff"));
    extentions.push_back(std::make_pair(".tr", "text/troff"));
    extentions.push_back(std::make_pair(".ts", "video/mp2t"));
    extentions.push_back(std::make_pair(".tsd", "application/timestamped-data"));
    extentions.push_back(std::make_pair(".tsv", "text/tab-separated-values"));
    extentions.push_back(std::make_pair(".ttc", "font/collection"));
    extentions.push_back(std::make_pair(".ttf", "font/ttf"));
    extentions.push_back(std::make_pair(".ttl", "text/turtle"));
    extentions.push_back(std::make_pair(".txt", "text/plain"));
    extentions.push_back(std::make_pair(".u8dsn", "message/global-delivery-status"));
    extentions.push_back(std::make_pair(".u8hdr", "message/global-headers"));
    extentions.push_back(std::make_pair(".u8mdn", "message/global-disposition-notification"));
    extentions.push_back(std::make_pair(".u8msg", "message/global"));
    extentions.push_back(std::make_pair(".uri", "text/uri-list"));
    extentions.push_back(std::make_pair(".uris", "text/uri-list"));
    extentions.push_back(std::make_pair(".urls", "text/uri-list"));
    extentions.push_back(std::make_pair(".vcard", "text/vcard"));
    extentions.push_back(std::make_pair(".vrml", "model/vrml"));
    extentions.push_back(std::make_pair(".vtt", "text/vtt"));
    extentions.push_back(std::make_pair(".vxml", "application/voicexml+xml"));
    extentions.push_back(std::make_pair(".war", "application/java-archive"));
    extentions.push_back(std::make_pair(".wasm", "application/wasm"));
    extentions.push_back(std::make_pair(".wav", "audio/wav"));
    extentions.push_back(std::make_pair(".weba", "audio/webm"));
    extentions.push_back(std::make_pair(".webm", "video/webm"));
    extentions.push_back(std::make_pair(".webmanifest", "application/manifest+json"));
    extentions.push_back(std::make_pair(".webp", "image/webp"));
    extentions.push_back(std::make_pair(".wgt", "application/widget"));
    extentions.push_back(std::make_pair(".wmf", "image/wmf"));
    extentions.push_back(std::make_pair(".woff", "font/woff"));
    extentions.push_back(std::make_pair(".woff2", "font/woff2"));
    extentions.push_back(std::make_pair(".wrl", "model/vrml"));
    extentions.push_back(std::make_pair(".wsdl", "application/wsdl+xml"));
    extentions.push_back(std::make_pair(".wspolicy", "application/wspolicy+xml"));
    extentions.push_back(std::make_pair(".x3d", "model/x3d+xml"));
    extentions.push_back(std::make_pair(".x3db", "model/x3d+binary"));
    extentions.push_back(std::make_pair(".x3dbz", "model/x3d+binary"));
    extentions.push_back(std::make_pair(".x3dv", "model/x3d+vrml"));
    extentions.push_back(std::make_pair(".x3dvz", "model/x3d+vrml"));
    extentions.push_back(std::make_pair(".x3dz", "model/x3d+xml"));
    extentions.push_back(std::make_pair(".xaml", "application/xaml+xml"));
    extentions.push_back(std::make_pair(".xdf", "application/xcap-diff+xml"));
    extentions.push_back(std::make_pair(".xdssc", "application/dssc+xml"));
    extentions.push_back(std::make_pair(".xenc", "application/xenc+xml"));
    extentions.push_back(std::make_pair(".xer", "application/patch-ops-error+xml"));
    extentions.push_back(std::make_pair(".xht", "application/xhtml+xml"));
    extentions.push_back(std::make_pair(".xhtml", "application/xhtml+xml"));
    extentions.push_back(std::make_pair(".xhvml", "application/xv+xml"));
    extentions.push_back(std::make_pair(".xm", "audio/xm"));
    extentions.push_back(std::make_pair(".xml", "application/xml"));
    extentions.push_back(std::make_pair(".xop", "application/xop+xml"));
    extentions.push_back(std::make_pair(".xpl", "application/xproc+xml"));
    extentions.push_back(std::make_pair(".xsd", "application/xml"));
    extentions.push_back(std::make_pair(".xsl", "application/xml"));
    extentions.push_back(std::make_pair(".xslt", "application/xslt+xml"));
    extentions.push_back(std::make_pair(".xspf", "application/xspf+xml"));
    extentions.push_back(std::make_pair(".xvm", "application/xv+xml"));
    extentions.push_back(std::make_pair(".xvml", "application/xv+xml"));
    extentions.push_back(std::make_pair(".yaml", "text/yaml"));
    extentions.push_back(std::make_pair(".yang", "application/yang"));
    extentions.push_back(std::make_pair(".yin", "application/yin+xml"));
    extentions.push_back(std::make_pair(".yml", "text/yaml"));
    extentions.push_back(std::make_pair(".zip", "application/zip"));
    
    if (_http_headers.find("Content-Type") != _http_headers.end())
    {
        for (std::vector<std::pair<std::string, std::string> >::iterator it = extentions.begin();
            it < extentions.end(); it++)
        {
            if (_http_headers["Transfer-Encoding"] == it->second)
            {
                _extention = it->first;
                break ;
            }
        }
    }
}