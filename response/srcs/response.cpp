#include "../includes/response.hpp"


// if the hanlde method returns 1 the request will be passed to next handler
// if the handle method returns 0 the request will be handled by the calling object



response::response()
{

}

response::~response()
{

}

responseHandler::responseHandler(client *_cl)
{
	cl=	_cl;
	buffer = "";
	response_body = "";
	headers = "";
	statusLine = "";
	error = 0;
}


std::string workingLocation::getDefaultError(int erroCode)
{
	int size;
	std::string errorstring;
	std::stringstream stream;
	stream << erroCode;
	stream >> errorstring;
	size = defaultError.size();
	for (int i = 0; i < size; i++)
	{
		if (defaultError[i][1] == errorstring)
		{
			//std::cout << defaultError[i][0] << std::endl;
			return (defaultError[i][0]);
		}
	}
	return ("");
}

std::string getResponseStatusLine(int status, std::string status_line)
{
	std::string buffer;

	buffer = HTTP_VERSION_1_1 + std::string(" ") + status_line + std::string("\n");
	return buffer;
}

bool isError(int status)
{
	if (status < 400)
		return false;
	return (true);
}

responseHandler::responseHandler(): cl()
{
	buffer = "";
	response_body = "";
	headers = "";
	statusLine = "";
	error = 0;
}

responseHandler::~responseHandler()
{
}


std::string responseHandler::getBuffer()
{
	return (statusLine + headers + response_body);
}

location *workingLocation::getLocation()
{
	return serverlocation;
}

void workingLocation::setUpload(std::string path)
{
	upload = path;
}


system_block_response::system_block_response(client *cl): responseHandler(cl)
{
}


int rmtree(const char path[])
{
    size_t path_len;
    char *full_path;
    DIR *dir;
    struct stat stat_path, stat_entry;
    struct dirent *entry;

    // stat for the path
    stat(path, &stat_path);

    // if path does not exists or is not dir - exit with status -1
    if (S_ISDIR(stat_path.st_mode) == 0) {
        fprintf(stderr, "%s: %s\n", "Is not directory", path);
        return (2);
    }

    // if not possible to read the directory for this user
    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "%s: %s\n", "Can`t open directory", path);
        return (2);
    }

    // the length of the path
    path_len = strlen(path);

    // iteration through entries in the directory
    while ((entry = readdir(dir)) != NULL) {

        // skip entries "." and ".."
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        // determinate a full path of an entry
        full_path = (char *)calloc(path_len + strlen(entry->d_name) + 1, sizeof(char));
        strcpy(full_path, path);
        strcat(full_path, "/");
        strcat(full_path, entry->d_name);

        // stat for the entry
        stat(full_path, &stat_entry);

        // recursively remove a nested directory
        if (S_ISDIR(stat_entry.st_mode) != 0) {
            if (rmtree(full_path) == 2)
				return (2);
			continue;
        }

        // remove a file object
        if (unlink(full_path) == 0)
            printf("Removed a file: %s\n", full_path);
        else
            return (2);
        free(full_path);
    }
    // remove the devastated directory and close the object of it
    if (rmdir(path) == 0)
        printf("Removed a directory: %s\n", path);
    else
        return (2);

    closedir(dir);
	return (1);
}

std::string    formatted_time(void)
{
    time_t    current;
    struct tm * timeinfo;
    char    buffer[80];

    time(&current);
    timeinfo = localtime (&current);

    strftime(buffer, 80, "%a,%e %b %Y %X %Z", timeinfo);

    return std::string(buffer);
}

std::string getResponseBody(int status, std::string bodyMsg, Locator *info)
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "getResponseBody: " << status << std::endl;
	std::string defaultEr = "";
	if (isError(status))
	{
		defaultEr = getDefaultError(status, info);
		if (defaultEr != "")
			return (defaultEr);
	}
	return (bodyMsg);
}

std::string getDefaultError(int status, Locator *info)
{
	std::string body = "";
	std::string path;
	//std::cout << "getDefaultError:returns full erro body page " << status << std::endl;
	if (status >= 400)
	{
		path = info->getWorkingLocation()->getDefaultError(status);
		if (path != std::string(""))
			body = info->readBody(path);
	}
	return (body);
}

std::string getResponseHeaders(int status, Locator *info, int body_size)
{
	std::string headers;
	std::stringstream s;
	std::string ss;
	s << body_size;
	s >> ss;
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "From getResponseHeaders" << std::endl;
	
	if (status != NO_CONTENT)
		headers += "content-length: " + std::string(ss) + std::string("\n");
	headers += "Server: " + std::string("420 SERVER") + std::string("\n");
	headers += "date: " + formatted_time() + std::string("\n");
	if (status >= 400)
		headers += "content-type: " + std::string("text/html") + std::string("\n");
	else
		headers += "content-type: " + info->getContentType() + std::string("\n");
	if (status == MOVED_PERMANENTLY)
	{
		int i = info->isredirection();
		if (i != -1)
			headers += "location: " + info->getWorkingLocation()->getRedirections()[i][1] + std::string("\n");
		else
			headers += "location: " + info->getResourceFullPath() + std::string("\n");
	}
	else if (status == CREATED_CODE)
		headers += "location: " + info->getResourceFullPath() + std::string("\n");//to be tested
	std::map<std::string, std::string> headersmap =  info->getClient()->getReadyRequest()->get_request_parsing_data().get_http_headers();
	std::map<std::string, std::string>::iterator it = headersmap.find("connection"); 
	if (it != headersmap.end())
		headers += "connection " + it->second + std::string("\n");
	headers += "\n";
	return headers;
}

std::string getResponseHeaders(int status, int body_size)
{
	std::string headers;
	std::stringstream s;
	std::string ss;
	s << body_size;
	s >> ss;
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "getResponseHeaders for errors: " << status << std::endl;

	if (status != NO_CONTENT)
		headers += "content-length: " + std::string(ss) + std::string("\n");
	headers += "connection: close\n";
	headers += "Server: " + std::string("420 SERVER") + std::string("\n");
	headers += "content-type: " + std::string("text/html") + std::string("\n");
	headers += "date: " + formatted_time() + std::string("\n");
	headers += "\n";
	return headers;
}

client *responseHandler::getClient(void)
{
	return cl;
}

std::string Locator::getContentType(void)
{
	std::string res = "";
	size_t i = resourceFullPath.find_last_of('.');
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "getContentType: " << resourceFullPath << std::endl;

	if (error >= 400)
		return ("text/html");
	if (i != std::string::npos)
	{
		res = resourceFullPath.substr(i, resourceFullPath.length() - i);
		//std::cout << res << std::endl;
	}
    if (!res.empty())
	{
		if (res == ".html") return "text/html";
		else if (res == ".htm") return "text/html";
		else if (res == ".shtml") return "text/html";
		else if (res == ".css") return "text/css";
		else if (res == ".xml") return "text/xml";
		else if (res == ".csv") return "text/csv";
		else if (res == ".gif") return "image/gif";
		else if (res == ".ico") return "image/x-icon";
		else if (res == ".jpeg") return "image/jpeg";
		else if (res == ".jpg") return "image/jpeg";
		else if (res == ".js") return "application/javascript";
		else if (res == ".json") return "application/json";
		else if (res == ".png") return "image/png";
		else if (res == ".pdf") return "application/pdf";
		else if (res == ".svg") return "image/svg+xml";
		else if (res == ".txt") return "text/plain";
		else if (res == ".atom") return "application/atom+xml";
		else if (res == ".rss") return "application/rss+xml";
		else if (res == ".webp") return "image/webp";
		else if (res == ".3gpp") return	"video/3gpp";
		else if (res == ".3gp") return "video/3gpp";
		else if (res == ".ts") return "video/mp2t";
		else if (res == ".mp4") return "video/mp4";
		else if (res == ".mpeg") return "video/mpeg";
		else if (res == ".mpg") return "video/mpeg";
		else if (res == ".mov") return "video/quicktime";
		else if (res == ".webm") return "video/webm";
		else if (res == ".flv") return "video/x-flv";
		else if (res == ".m4v") return "video/x-m4v";
		else if (res == ".mng") return "video/x-mng";
		else if (res == ".asx") return "video/x-ms-asf";
		else if (res == ".asf") return "video/x-ms-asf";
		else if (res == ".wmv") return "video/x-ms-wmv";
		else if (res == ".avi") return "video/x-msvideo";
		else if (res == ".mml") return "text/mathml";
		else if (res == ".txt") return "text/plain";
		else if (res == ".jad") return "text/vnd.sun.j2me.app-descriptor";
		else if (res == ".wml") return "text/vnd.wap.wml";
		else if (res == ".htc") return "text/x-component";
		else if (res == ".png") return "image/png";
		else if (res == ".tif") return "image/tiff";
		else if (res == ".tiff") return "image/tiff";
		else if (res == ".wbmp") return "image/vnd.wap.wbmp";
		else if (res == ".ico") return "image/x-icon";
		else if (res == ".jng") return "image/x-jng";
		else if (res == ".bmp") return "image/x-ms-bmp";
		else if (res == ".svg") return "image/svg+xml";
		else if (res == ".svgz") return "image/svg+xml";
		else if (res == ".mid") return "audio/midi";
		else if (res == ".midi") return "audio/midi";
		else if (res == ".kar") return "audio/midi";
		else if (res == ".mp3") return "audio/mpeg";
		else if (res == ".ogg") return "audio/ogg";
		else if (res == ".m4a") return "audio/x-m4a";
		else if (res == ".ra") return "audio/x-realaudio";
		else if (res == ".pl") return "application/x-perl";
		else if (res == ".py") return "application/x-python";
		else if (res == ".php") return "application/x-php";
		else if (res == ".cpp") return "application/x-c++";
		else if (res == ".c") return "application/x-c";
	}
    return "plain/text";
}


void responseHandler::setClient(client *_cl)
{
	cl = _cl;
}

int system_block_response::handle()
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "system_block_response HANDLE" << std::endl;
	int err = 0;
 	if (this->isMethodImplemented(cl->getReadyRequest()->get_request_parsing_data().get_http_method()))
		err = 1;
	else if (this->isHttpVersionSupported(cl->getReadyRequest()->get_request_parsing_data().get_http_version()) == 1)
		err = 1;
	else if (cl->getReadyRequest()->get_request_parsing_data().get_code_status() == 500)
	{
		err = 1;
		error = INTERNAL_SERVER_ERROR_CODE;
	}
	else
		return (0);
	if (err == 1)
		this->buildresponse();
	return 1;
}

int system_block_response::isMethodImplemented(std::string method)
{
	if (method == GET || method == POST || method == DELETE)
		return 0;
	error = NOT_IMPLEMENTED_CODE;
	return 1;
}

int system_block_response::isHttpVersionSupported(std::string http_version)
{
	if (http_version == HTTP_VERSION_1_1)
		return 0;
	error = HTTP_VERSION_NOT_SUPPORTED_CODE;
	return 1;
}

void system_block_response::buildresponse()
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "system_block_response BUILD" << std::endl;
	switch (error)
	{
		case HTTP_VERSION_NOT_SUPPORTED_CODE:
			response_body = getErroBody(error, HTTP_VERSION_NOT_SUPPORTED_RESPONSE_MSG, cl);
			statusLine = getResponseStatusLine(error, HTTP_VERSION_NOT_SUPPORTED_MSG);		
			headers = 	getResponseHeaders(error, response_body.size());
			break;
		case NOT_IMPLEMENTED_CODE:
			response_body = getErroBody(error, NOT_IMPLEMENTED_RESPONSE_MSG, cl);
			statusLine = getResponseStatusLine(error, NOT_IMPLEMENTED_MSG);		
			headers = 	getResponseHeaders(error, response_body.size());
			break;
		case INTERNAL_SERVER_ERROR_CODE:
			response_body = getErroBody(error, INTERNAL_SERVER_ERROR_RESPONSE_MSG, cl);
			statusLine = getResponseStatusLine(error, INTERNAL_SERVER_ERROR_MSG);		
			headers = 	getResponseHeaders(error, response_body.size());
			break;
		default:
			return;
	}
}



// each location in the server block has its own path
// we will match the path of the request with the path of each location and chose the location with the longest match
//Nginx begins by checking all prefix-based location matches.
//It checks each location against the complete request URI.
workingLocation::workingLocation()
{
	serverlocation = NULL;
	upload = "";
	cgiLocation = NULL;
}

// copy constructor
workingLocation::workingLocation(const workingLocation &wl)
{
	serverlocation = wl.serverlocation;
	upload = wl.upload;
	cgiLocation = wl.cgiLocation;
}

workingLocation::~workingLocation()
{
	delete serverlocation;
	delete cgiLocation;
}


location* workingLocation::searchLocation(std::vector<location> locations, std::string source, server *server)
{
	int max_match_length = 0;
	location *loc = new location();
	int	match;
	int size = locations.size();
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "workingLocation SEARCH" << std::endl;
	for (int i = 0; i < size; i++)
	{
		match = 0;
		for (int j = 0; j < locations[i].get_locations_path().size(); j++)
		{
			if (locations[i].get_locations_path()[j] == source[j])
				match++;
			else
				break;
		}
		if (match > max_match_length)
		{
			max_match_length = match;
			*loc = locations[i];
		}
	}
	if (max_match_length == 0)
	{
		//std::cout << "working location not found" << std::endl;
		delete loc;
		return (NULL);
	}
	longest_match = max_match_length;
	checkLocation(server);
	return loc;
}

void workingLocation::setlocation(request *request)
{

	location *loc = this->searchLocation(request->get_server()->get_location(), request->get_request_parsing_data().get_http_path(), request->get_server());

	//std::cout << "==========================================================" << std::endl;
	//std::cout << "workingLocation SET" << std::endl;
	if (loc == NULL)
	{
		loc = this->defaultLocation(request->get_server());
	}
	serverlocation = loc;
	setUpload(request->get_server()->get_upload_path());
	// testing if setUpload is working
	//std::cout << "testing if setUpload is working" << std::endl;
	//std::cout << "upload path: " << upload << std::endl;
	setRedirections(request->get_server());
	setDefaultError(request->get_server()); 
	if (request->get_server()->get_cgi_size() != 0)
		setCgi(request->get_server()->get_cgi(0));
}

void workingLocation::setDefaultError(server *server)
{
	defaultError = server->get_error_pages();
}

void workingLocation::setRedirections(server *server)
{
	redirections = server->get_redirections();
}


// NEEDS MORE DETAILS UPLOAD PATH ECT MAYBE ADD CGI TO A LOCATION
	// think about an other way than this location to store default data
location *workingLocation::defaultLocation(server *server)
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "workingLocation DEFAULT" << std::endl;
	//std::cout << "default location" << std::endl;
	location *loc = new location();
	loc->set_root(server->get_root());
	loc->set_client_max_body_size(server->get_client_max_body_size());
	// loc->set_index(server->get_index());
	loc->fill_index(server->get_index(), 0);
	loc->set_autoindex(server->get_autoindex());
	loc->fill_allowed_methods(server->get_allowed_methods(), 0);
	return loc;
}


std::string workingLocation::getUpload(void)
{
	return (upload);
}

std::vector<std::vector<std::string> > workingLocation::getDefaultError(void)
{
	return defaultError;
}

std::vector<std::vector<std::string> > workingLocation::getRedirections()
{
	return redirections;
}


Locator::Locator(client *_cl): responseHandler(_cl)
{
	// this->setClient(cl);
	Locate = new workingLocation;
	resourceFullPath = "";
	hasIndex = false;
	autoindex = false;
	hasIndex = false;
	statusLine = "";
	headers = "";
	response_body = "";
}

Locator::~Locator()
{
	delete Locate;
}

void Locator::setResourceFullPath()
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "Locator SET RESOURCE FULL PATH" << std::endl;
	std:: string p = cl->getReadyRequest()->get_request_parsing_data().get_http_path();
	int i;
	//=================================//
	std::string tmp = Locate->getUpload();
	if (tmp != "" && tmp[0] == '.')
	{
		tmp.erase(0, 1);
		//std::cout << "tmp: " << tmp << std::endl;
		//std::cout << p.find(tmp) << std::endl;
		//std::cout << p << std::endl;
		if (p.find(tmp) != -1)
		{
			//std::cout << "upload path found" << std::endl;
			resourceFullPath = cl->getReadyRequest()->get_server()->get_root() + Locate->getUpload();
			resourceFullPath += filter(p);
			//=================================//
		}
	}
	else
	{
		resourceFullPath = Locate->getLocation()->get_root();
		//std::cout << "resourceFullPath befor : " << resourceFullPath << std::endl;
		std::string loc = Locate->getLocation()->get_locations_path();
		p.erase(0, Locate->getLongest_match());
		if (p[0] == '/')
			p.erase(0, 1);
		resourceFullPath += p;
	}
}

std::string Locator::getResourceFullPath(void)
{
	return resourceFullPath;
}

void PostHandler::setClient(client *_cl)
{
	cl = _cl;
}

system_block_response::~system_block_response()
{

}

GetHandler::~GetHandler()
{
}


PostHandler::~PostHandler()
{
}


void GetHandler::setClient(client *_cl)
{
	cl = _cl;
}

void DeleteHandler::setClient(client *_cl)
{
	cl = _cl;
}

void Locator::setClient(client *_cl)
{
	cl = _cl;
}


void Locator::checker(void)
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "Locator CHECKER" << std::endl;
	struct stat s;
	std::string path = cl->getReadyRequest()->get_request_parsing_data().get_http_path();
	setworkingLocation();
	setResourceFullPath();
	setAutoIndex();
	//std::cout << resourceFullPath << std::endl;

	if (isCgi(resourceFullPath))
	{
		//std::cout << "file is cgi" << std::endl;
		resourceType = CG;
	}
	else if (stat(resourceFullPath.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			//std::cout << "file is a directory" << std::endl;
			resourceType = DIRE;
			setIndex();
		}
		else
		{
			//std::cout << "file is not a directory" << std::endl;
			resourceType = FI;
		}
	}
	else
	{
		//std::cout << "file not found" << std::endl;
		resourceType = NO;
	}
	if (resourceFullPath[resourceFullPath.size() - 1] == '/')
	{
		//std::cout << "file end with slaches" << std::endl;
		endwithslash = true;
	}
	else
	{
		//std::cout << "file dose not end with slaches" << std::endl;
		endwithslash = false;
	}
	// prinf all Locator data
	//std::cout << "resource type: " << resourceType << std::endl;
	//std::cout << "resource full path: " << resourceFullPath << std::endl;
	//std::cout << "autoindex: " << autoindex << std::endl;
	//std::cout << "has index: " << hasIndex << std::endl;
	//std::cout << "endwithslash: " << endwithslash << std::endl;
	//std::cout << "indexfile: " << indexfile << std::endl;
	// print all workingLocation data
	//std::cout << "Upload: " << Locate->getUpload() << std::endl;
	//print all workingLocation::serverlocation data
	//std::cout << "root: " << Locate->getLocation()->get_root() << std::endl;
	//std::cout << "client max body size: " << Locate->getLocation()->get_client_max_body_size() << std::endl;
	//std::cout << "autoindex: " << Locate->getLocation()->get_autoindex() << std::endl;
	//std::cout << "location_path: " << Locate->getLocation()->get_locations_path() << std::endl;
	//std::cout << "==========================================================" << std::endl;
}


int Locator::HandleCGI()
{
	execute_cgi cgiHandler;

	//std::cout << "==========================================================" << std::endl;
	//std::cout << "Locator HANDLE CGI ***********************" << std::endl;
	if (cgiHandler.start_execute_cgi(resourceFullPath, getWorkingLocation()->getCgi()->get_cgi_path(), cl->getReadyRequest()->get_request_parsing_data()) != 1)
		statusLine = getResponseStatusLine(500, INTERNAL_SERVER_ERROR_MSG);
	else
		statusLine = getResponseStatusLine(200, OK_MSG);
	//std::cout << "cgi full path "<< cgiHandler.get_file_body_path() << std::endl;
	response_body = readBody(cgiHandler.get_file_body_path());
	//std::cout <<  " response dyal cgi dzebbi \n "<< response_body <<  std::endl;
	//std::cout << "==========================================================" << std::endl;
	std::map <std::string, std::string> tmp = cgiHandler.get_headers();
	for(std::map <std::string, std::string>::iterator it = tmp.begin(); it!=tmp.end(); ++it)
		headers += it->first + ": " + it->second + "\r\n";
	// std::cout << "headers :" << headers << std::endl;
	error = -1337;
	return (error);// if error is -1337 then cgi is executed
}

std::string Locator::getindexfile()
{
	return indexfile;
}

std::string Locator::setindexfile(void)
{
	std::string indexfile;
	std::vector<std::string> _indexs = getWorkingLocation()->getLocation()->get_index();
	int size = _indexs.size();
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to setindexfile" << std::endl;
	struct stat s;
	for (int i = 0; i < size; i++)
	{
		indexfile = getWorkingLocation()->getLocation()->get_root() + _indexs[i];
		if (stat(indexfile.c_str(), &s) == 0)
		{
			//std::cout << "indexfile " << indexfile << std::endl;
			if (!(s.st_mode & S_IFDIR))
				return (_indexs[i]);
		}
	}
	return (std::string("NON"));
}

void Locator::setworkingLocation(void)
{
	this->Locate->setlocation(cl->getReadyRequest());
}


bool Locator::isMethodAllowd(std::string method)
{
	std::vector<std::string> it = Locate->getLocation()->get_methods();
	for (int i = 0; i < it.size(); i++)
	{
		if (method == it[i])
			return (true);
	}
	return (false);
}

workingLocation *Locator::getWorkingLocation(void)
{
	return Locate;
}

cgi *workingLocation::getCgi(void)
{
	return (cgiLocation);
}

void workingLocation::setCgi(cgi Cgi)
{
	cgiLocation = new cgi;
	*cgiLocation = Cgi;
}



std::string readBody(std::string path)
{
	std::fstream in_file(path);
	std::string body = "";
	struct stat sb;

	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to readBody" << std::endl;
    FILE* input_file = fopen(path.c_str(), "r");
    if (input_file == nullptr) {
		return (body);
    }
	stat(path.c_str(), &sb);
    body.resize(sb.st_size);
    fread(const_cast<char*>(body.data()), sb.st_size, 1, input_file);
    fclose(input_file);

    return body;
}


int Locator::getResourceType(void)
{
	return (resourceType);
}

std::string getErroBody(int erroCode, std::string definebody, client *cl)
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to getErroBody" << std::endl;
	if (erroCode < 400)
		return (definebody);
	std::vector<std::vector<std::string> > defaultError = cl->getReadyRequest()->get_server()->get_error_pages();
	int size;
	std::string errorstring;
	std::stringstream stream;
	std::string path = std::string("");

	stream << erroCode;
	stream >> errorstring;
	size = defaultError.size();
	for (int i = 0; i < size; i++)
	{
		if (defaultError[i][0] == errorstring)
			path = defaultError[i][1];
	}
	if (path != std::string(""))
		return (readBody(path));
	return (definebody); 
}

std::string Locator::readBody(std::string path)
{
	// //std::cout << "==========================================================" << path << std::endl;
	// struct stat sb;
	// std::string body;

    // FILE* input_file = fopen(path.c_str(), "r");
	// //std::cout << strerror(errno) << std::endl;
    // if (input_file == nullptr) {
	// 	//std::cout <<"%%%%%%%%%%%%%%%%%%%" << std::endl;
	// 	return (body);
    // }
	// stat(path.c_str(), &sb);
    // body.resize(sb.st_size);
    // fread(const_cast<char*>(body.data()), sb.st_size, 1, input_file);
    // fclose(input_file);

 	struct stat sb;
    std::string res;

    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0) {
        perror("open\n");
    }
    fstat(fd, &sb);
	//std::cout << sb.st_size << std::endl;
    res.resize(sb.st_size);
    read(fd, (char*)(res.data()), sb.st_size);
    close(fd);
    return res;
	// int fd = open(path.c_str(), O_RDONLY);
	// if (fd == -1)
	// 	//std::cout << strerror(errno) << std::endl;
}

int Locator::isredirection()
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to isredirection" << std::endl;
	std::vector<std::vector<std::string> > redirections = getWorkingLocation()->getRedirections();
	int size = redirections.size();
	for (int i = 0; i < size; i++)
	{
		std::vector<std::string> it = redirections[i];
		if (it[0] == cl->getReadyRequest()->get_request_parsing_data().get_http_path())
		{// need update
			//std::cout << it[0] << std::endl;
			return (i);
		}
	}
	//std::cout << "		return -1" << std::endl;
	return (-1);
}

int Locator::handle()
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to handle::Locator" << std::endl;
	error = 0;
	if (!isMethodAllowd(cl->getReadyRequest()->get_request_parsing_data().get_http_method()))
		error = NOT_ALLOWED_CODE;
	else if (cl->getReadyRequest()->get_request_parsing_data().get_code_status() == 400)
		error = RESPONSE_BAD_REQUEST;
	else if (isredirection() != -1)
		error = MOVED_PERMANENTLY;
	if (error != 0)
	{
		buildresponse();
		return (1);	
	}
	return (0);
}


bool Locator::isCgi(std::string path)
{
	if (Locate->getCgi() == NULL)
	{
		return (false);
	}
	// //std::cout << "inside isCgi" << cl->getReadyRequest()->get_request_parsing_data().get_http_path() << std::endl;
	if (endsWith(path, ".php")) //cl->getReadyRequest()->get_request_parsing_data().get_http_path()
	{
		//std::cout << "==========================================================9999" << std::endl;
		return true;
	}	
	return false;
}

void Locator::setResourceFullPath(std::string path)
{
	resourceFullPath = path;
}

void Locator::buildresponse()
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to buildresponse from locator" << std::endl;
	switch (error)
	{
	case RESPONSE_BAD_REQUEST:
		response_body = getResponseBody(RESPONSE_BAD_REQUEST, BAD_REQUEST_RESPONSE_MSG, this);
		statusLine = getResponseStatusLine(RESPONSE_BAD_REQUEST, BAD_REQUEST_MSG);
		headers = getResponseHeaders(RESPONSE_BAD_REQUEST, this, response_body.size());
		break;
	case NOT_ALLOWED_CODE:
		response_body = getResponseBody(NOT_ALLOWED_CODE, NOT_ALLOWED_RESPONSE_MSG, this);
		statusLine = getResponseStatusLine(NOT_ALLOWED_CODE, NOT_ALLOWED_MSG);
		headers = getResponseHeaders(NOT_ALLOWED_CODE, this, response_body.size());
		break;
	case MOVED_PERMANENTLY:
		response_body = getResponseBody(MOVED_PERMANENTLY, MOVED_PERMANENTLY_RESPONSE_MSG, this);
		statusLine = getResponseStatusLine(MOVED_PERMANENTLY, MOVED_PERMANENTLY_MSG);
		headers = getResponseHeaders(MOVED_PERMANENTLY, this, response_body.size());
		break;
	default:
		break;
	}
}

bool Locator::gedEnd(void)
{
	return endwithslash;
}

void Locator::setAutoIndex(void)
{
	autoindex = Locate->getLocation()->get_autoindex();
}

bool Locator::getAutoIndex(void)
{
	return autoindex;
}

bool Locator::getIndex()
{
	if (hasIndex)
		return (true);
	return (false);
}

GetHandler::GetHandler(Locator *_godFather): responseHandler(_godFather->getClient())
{
	godFather = _godFather;
}


void system_block_response::setClient(client *_cl)
{
	cl = _cl;
}

client *system_block_response::getClient(void)
{
	return (cl);
}

std::string getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host, int port) 
{
    std::stringstream   ss;
    ss << "\t\t<p><a href=\"http://" + host + ":" <<\
        port << dirName + "/" + dirEntry + "\">" + dirEntry + "</a></p>\n";
    return ss.str();
}

std::string GetHandler::setAutoindexResponse(void)
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to setAutoindexResponse" << std::endl;
	std::string path	= godFather->getResourceFullPath();
	std::string host	= cl->getReadyRequest()->get_server()->get_listen_host();
	int port			= cl->getReadyRequest()->get_server()->get_listen_port();

    std::string dirName(path);
    DIR *dir = opendir(path.c_str());
    std::string page =\
    "<!DOCTYPE html>\n\
    <html>\n\
    <head>\n\
            <title>" + dirName + "</title>\n\
    </head>\n\
    <body>\n\
    <h1>INDEX</h1>\n\
    <p>\n";

	if (dir == NULL)
	{
		error = INTERNAL_SERVER_ERROR_CODE;
		return ("");
	}
    if (dirName[0] != '/')
        dirName = "/" + dirName;
    for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir)) {
        page += getLink(std::string(dirEntry->d_name), dirName, host, port);
    }
    page +="\
    </p>\n\
    </body>\n\
    </html>\n";
    closedir(dir);

	return page;
}

void	Locator::setIndex(void)
{
	std::string index;

	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to setIndex" << std::endl;
	if (Locate->getLocation()->get_index().size() > 0)
	{
		indexfile = setindexfile();
		if (index == std::string("NON"))
			hasIndex = false;
		else
			hasIndex = true;
	}
	else
		hasIndex = false;
}

void GetHandler::setGodFather(Locator *_godFather)
{
	this->godFather = _godFather;
}

int GetHandler::handle()
{

	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to handle from gethandler" << std::endl;
	if (godFather->getResourceType() == NO)
	{
		error = NOT_FOUND_CODE;
	}
	else if (godFather->getResourceType() == FI || godFather->getResourceType() == CG)
	{
		handleFiles();
	}
	else
	{
		HandleDir();
	}
	buildresponse();
	return (1);
};

int GetHandler::HandleDir(void)
{
	std::string newpath;
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to GetHandler::HandleDir" << std::endl;
	if (!godFather->gedEnd())
	{
		//std::cout << "no slash" << std::endl;
		newpath = godFather->getResourceFullPath() + "/";
		godFather->setResourceFullPath(newpath);
		error = MOVED_PERMANENTLY;
	}
	else if (!godFather->getIndex())
	{
		//std::cout << "no index" << std::endl;
		if (godFather->getAutoIndex())
			error = AUTOINDEX_CODE;
		else
			error = FORBIDDEN_CODE;
	}
	else
	{
		newpath = godFather->getResourceFullPath() + godFather->getindexfile();
		//std::cout << "newpath" << newpath << std::endl;
		godFather->setResourceFullPath(newpath);
		handleFiles();
	}
	return (1);
}

int GetHandler::handleFiles()
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to Get::HandlerhandleFiles" << std::endl;
	if (godFather->isCgi(godFather->getResourceFullPath()))
	{
		godFather->HandleCGI();
		statusLine = godFather->getLine();
		response_body = godFather->getBody();
		headers = godFather->getHeaders();
	}
	else
		error = 200;
	return (1);
}

void GetHandler::buildresponse()
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to GetHandler::buildresponse" << std::endl;
	switch (error)
	{
	case AUTOINDEX_CODE:
		response_body = getResponseBody(200, setAutoindexResponse(), godFather);
		statusLine = getResponseStatusLine(AUTOINDEX_CODE, OK_MSG);
		headers = getResponseHeaders(200, godFather, response_body.size());
		break;
	case MOVED_PERMANENTLY:
		response_body = getResponseBody(MOVED_PERMANENTLY, MOVED_PERMANENTLY_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(MOVED_PERMANENTLY, MOVED_PERMANENTLY_MSG);
		headers = getResponseHeaders(MOVED_PERMANENTLY, godFather, response_body.size());
		break;
	case 200:
		response_body = getResponseBody(200, godFather->readBody(godFather->getResourceFullPath()), godFather);
		//std::cout << "response_body" << response_body << std::endl;
		statusLine = getResponseStatusLine(200, OK_MSG);
		headers = getResponseHeaders(200, godFather, response_body.size());
		break;
	case FORBIDDEN_CODE:
		response_body = getResponseBody(FORBIDDEN_CODE, FORBIDDEN_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(FORBIDDEN_CODE, FORBIDDEN_MSG);
		headers = getResponseHeaders(FORBIDDEN_CODE, godFather, response_body.size());
		break;
	case NOT_FOUND_CODE:
		response_body = getResponseBody(NOT_FOUND_CODE, NOT_FOUND_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(NOT_FOUND_CODE, FORBIDDEN_MSG);
		headers = getResponseHeaders(NOT_FOUND_CODE, godFather, response_body.size());
		break;
	default:
		break;
	}
}

DeleteHandler::DeleteHandler(Locator *_godFather): responseHandler(_godFather->getClient())
{
	godFather = _godFather;
}

void DeleteHandler::setGodFather(Locator *_godFather)
{
	godFather = _godFather;
}

DeleteHandler::~DeleteHandler()
{
}

int DeleteHandler::handle()
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to DeleteHandler::handle" << std::endl;
	if (godFather->getResourceType() == NO)
		error = NOT_FOUND_CODE;
	else if (godFather->getResourceType() == FI || godFather->getResourceType() == CG)
	{
		handleFiles();
	}
	else
		HandleDir();
	buildresponse();
	return (1);
};

int DeleteHandler::deleter(std::string path)
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to DeleteHandler::deleter" << std::endl;
	if (godFather->getResourceType() == DIRE)
	{
		error = rmtree(path.c_str());
		return (error);
	}
	else if (access(path.c_str(), W_OK) == 0)
	{
		remove(path.c_str());
		return (0);
	}
	else
		return ((error = 2));
}

int DeleteHandler::handleFiles(void)
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to DeleteHandler::handleFiles" << std::endl;
	if (godFather->isCgi(godFather->getResourceFullPath()))
	{
		godFather->HandleCGI();
		statusLine = godFather->getLine();
		response_body = godFather->getBody();
		headers = godFather->getHeaders();
	}
	else
	{
		if (deleter(godFather->getResourceFullPath()) == 0)
			error = NO_CONTENT;
		else if (error == 2)
			error = FORBIDDEN_CODE;
		else
			error = INTERNAL_SERVER_ERROR_CODE;//check this
			// not allowed or server error ? 
			//i guess a server error 
			//because if the resourse is found within a location, the user might delete it
	}
	return (1);
}

int DeleteHandler::HandleDir(void)
{
	std::string newpath;

	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to DeleteHandler::HandleDir" << std::endl;
	if (!godFather->gedEnd())
		error = CONFLICT;
	else if (godFather->getWorkingLocation()->getCgi() != NULL)
	{
		if (godFather->getIndex())
		{
			newpath = godFather->getResourceFullPath() + godFather->getindexfile();
			godFather->setResourceFullPath(newpath);
			handleFiles();
		}
		else
			error = FORBIDDEN_CODE;
	}
	else
	{
		if (deleter(godFather->getResourceFullPath()) == 0)
			error = NO_CONTENT;//IF SUCCES ==> NO_CONTENT
		else if (error == 2) // this variable may cause a problem if it got changed by deleter method
			error = FORBIDDEN_CODE;//	PERMITION ERROR ==> FORBIDEN
		else
			error = INTERNAL_SERVER_ERROR_CODE;
		error = 200;
	}
	return (1);
}


void DeleteHandler::buildresponse(void)
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to DeleteHandler::buildresponse" << std::endl;
	switch (error)
	{
	case NO_CONTENT:
		statusLine = getResponseStatusLine(NO_CONTENT, NO_CONTENT_MSG);
		headers = getResponseHeaders(NO_CONTENT, godFather, 0);
		break;
	case FORBIDDEN_CODE:
		response_body = getResponseBody(FORBIDDEN_CODE, FORBIDDEN_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(FORBIDDEN_CODE, FORBIDDEN_MSG);
		headers = getResponseHeaders(FORBIDDEN_CODE, godFather, response_body.size());
		break;
	case INTERNAL_SERVER_ERROR_CODE:
		response_body = getResponseBody(INTERNAL_SERVER_ERROR_CODE, INTERNAL_SERVER_ERROR_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(INTERNAL_SERVER_ERROR_CODE, INTERNAL_SERVER_ERROR_MSG);
		headers = getResponseHeaders(INTERNAL_SERVER_ERROR_CODE, godFather, response_body.size());
		break;
	case CONFLICT:
		response_body = getResponseBody(CONFLICT, CONFLICT_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(CONFLICT, CONFLICT_MSG);
		headers = getResponseHeaders(CONFLICT, godFather, response_body.size());
		break;
	case NOT_FOUND_CODE:
		response_body = getResponseBody(NOT_FOUND_CODE, NOT_FOUND_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(NOT_FOUND_CODE, FORBIDDEN_MSG);
		headers = getResponseHeaders(NOT_FOUND_CODE, godFather, response_body.size());
		break;
	default:
		break;
	}
}


PostHandler::PostHandler(Locator *_godFather): responseHandler(_godFather->getClient())
{
	godFather = _godFather;
}

void PostHandler::setGodFather(Locator *_godFather)
{
	godFather = _godFather;
}

bool PostHandler::supportAppload()
{
	if (godFather->getWorkingLocation()->getUpload() != std::string(""))
		return true;
	return false;
}

client *PostHandler::getClient(void)
{
	return (cl);
}


int PostHandler::creator(std::string path)
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to PostHandler::creator" << std::endl;

	//std::string newf = godFather->getWorkingLocation()->getLocation()->get_root();
	// if ((newf.find_last_of("/") != newf.size() - 1) && (godFather->getWorkingLocation()->getUpload().find_first_of("/") != 0))
	// 	newf += "/";
	// else if ((newf.find_last_of("/") != newf.size() - 1) && (godFather->getWorkingLocation()->getUpload().find_first_of("/") != 0))
	// 	newf.pop_back();
	std::string newf = godFather->getWorkingLocation()->getUpload();
	//std::cout << "newf 00: " << newf << std::endl;	
	if (newf.find_last_of("/") != newf.size() - 1)
		newf += "/";
	newf += getClient()->getReadyRequest()->get_request_parsing_data().get_path_body();
	//std::cout << "newf 01: " << newf << std::endl;	
	int new_fd = open(newf.c_str(), O_RDWR | O_CREAT | O_APPEND, 0644);
	int old_fd = open(getClient()->getReadyRequest()->get_request_parsing_data().get_path_body().c_str(), O_RDONLY, 0644);
	char buffer[2000] = {0};
	int ret = 1;

	if (new_fd == -1 || old_fd == -1)
		return (500);
	lseek(old_fd, 0, SEEK_SET);
    while (ret > 0)
	{
		memset(buffer, 0, 2000);
		ret = read(old_fd, buffer, 2000 - 1);
		if (ret >= 0)
			write(new_fd, buffer, ret);
        else
            return 500;
	}
	godFather->setResourceFullPath(newf);
	close(new_fd);
	close(old_fd);
	unlink(getClient()->getReadyRequest()->get_request_parsing_data().get_path_body().c_str());
	remove(getClient()->getReadyRequest()->get_request_parsing_data().get_path_body().c_str());
	return (1);
}

int PostHandler::handle()
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to PostHandler::handle" << std::endl;
	//std::cout << "support upload " << supportAppload() << std::endl;
	if (supportAppload())
	{	
		if (creator(godFather->getResourceFullPath()) == 500)
			error = INTERNAL_SERVER_ERROR_CODE;
		else
			error = CREATED_CODE;
		//std::cout << "==========================================================" << error << std::endl;
	}
	else
	{
		if (godFather->getResourceType() == FI || godFather->getResourceType() == CG)
			handleFiles();
		else if (godFather->getResourceType() == DIRE)
			HandleDir();
		else
			error = NOT_FOUND_CODE;
	}
	buildresponse();
	return (1);
}

int PostHandler::handleFiles(void)
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to PostHandler::handleFiles" << std::endl;
	if (godFather->isCgi(godFather->getResourceFullPath()))
	{
		//std::cout << "		is cgi" << std::endl;
		godFather->HandleCGI();
		statusLine = godFather->getLine();
		response_body = godFather->getBody();
		headers = godFather->getHeaders();
	}
	else
		error = FORBIDDEN_CODE;
	return (1);
}

int PostHandler::HandleDir(void)
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to PostHandler::HandleDir" << std::endl;
	std::string newpath;
	if (godFather->gedEnd())
	{
		if (godFather->getIndex())
		{
			//std::cout << "index" << std::endl;
			newpath = godFather->getResourceFullPath() + godFather->getindexfile();
			//std::cout << "newpath: " << newpath << std::endl;
			godFather->setResourceFullPath(newpath);
			handleFiles();
		}
		else
		{
			//std::cout << "no index files" << std::endl;
			error = FORBIDDEN_CODE;
		}
	}
	else
	{
		//append slach
		newpath = godFather->getResourceFullPath() + "/";
		godFather->setResourceFullPath(newpath);
		error = MOVED_PERMANENTLY;
	}
	return (1);
}

void PostHandler::buildresponse()
{
	//std::cout << "==========================================================" << std::endl;
	//std::cout << "	call to PostHandler::buildresponse" << std::endl;
	switch (error)
	{
	case CREATED_CODE:
		response_body = getResponseBody(CREATED_CODE, CREATED_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(CREATED_CODE, CREATED_MSG);
		headers = getResponseHeaders(CREATED_CODE, godFather, response_body.size());
		break;
	case NOT_FOUND_CODE:
		response_body = getResponseBody(NOT_FOUND_CODE, NOT_FOUND_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(NOT_FOUND_CODE, FORBIDDEN_MSG);
		headers = getResponseHeaders(NOT_FOUND_CODE, godFather, response_body.size());
		break;
	case FORBIDDEN_CODE:
		response_body = getResponseBody(FORBIDDEN_CODE, FORBIDDEN_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(FORBIDDEN_CODE, FORBIDDEN_MSG);
		headers = getResponseHeaders(FORBIDDEN_CODE, godFather, response_body.size());
		break;
	case MOVED_PERMANENTLY:
		response_body = getResponseBody(MOVED_PERMANENTLY, MOVED_PERMANENTLY_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(MOVED_PERMANENTLY, MOVED_PERMANENTLY_MSG);
		headers = getResponseHeaders(MOVED_PERMANENTLY, godFather, response_body.size());
		break;
	case INTERNAL_SERVER_ERROR_CODE:
		response_body = getResponseBody(INTERNAL_SERVER_ERROR_CODE, INTERNAL_SERVER_ERROR_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(INTERNAL_SERVER_ERROR_CODE, INTERNAL_SERVER_ERROR_MSG);
		headers = getResponseHeaders(INTERNAL_SERVER_ERROR_CODE, godFather, response_body.size());
		break;
	default:
		break;
	}
}


responseHandler *getResponse(client  *cl)
{
	responseHandler *systemResponse = new system_block_response(cl);
	Locator *locationHandler = new Locator(cl);
	std::string method = cl->getReadyRequest()->get_request_parsing_data().get_http_method();

	if (systemResponse->handle())
	{
		//std::cout << "system response handle" << std::endl;
		delete locationHandler;
		return (systemResponse);
	}
	else
		locationHandler->checker();	
	if (locationHandler->handle())
	{
		//std::cout << "location handler" << std::endl;
		//std::cout << locationHandler->getBuffer() << std::endl;
		delete systemResponse;
		return locationHandler;
	}
	if (method == std::string("POST"))
	{
		//std::cout << "PostHandler" << std::endl;

		responseHandler *_postHandler = new PostHandler(locationHandler);
		_postHandler->handle();
		delete locationHandler;
		delete systemResponse;
		return (_postHandler);
	}
	if (method == std::string("GET"))
	{
		//std::cout << "GetHandler" << std::endl;
		responseHandler *_getHandler = new GetHandler(locationHandler);
		_getHandler->handle();
		//std::cout << "check if buffer is empty:" <<  _getHandler->getBuffer() << std::endl;
		//std::cout << _getHandler->getBuffer() << std::endl;
		delete locationHandler;
		delete systemResponse;
		return (_getHandler);
	}
	else
	{
		//std::cout << "DeleteHandler" << std::endl;		
		responseHandler *_deleteHandler = new DeleteHandler(locationHandler);
		_deleteHandler->handle();
		delete locationHandler;
		delete systemResponse;
		return _deleteHandler;
	}
	return (0);
}

bool endsWith(std::string const str, std::string const suffix)
{
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.rfind(suffix) == str.size() - suffix.size();
}

std::string filter(std::string path)
{
	int flag = 0;

	if (path == std::string("/"))
		return (path);
	if (endsWith(path, "/"))
	{
		flag = 1;
		path.erase(path.size() - 1);
	}
	int last = path.find_last_of("/");
	if (last != -1)
	{
		path = path.substr(last + 1);
	}
	if (flag)
		path += "/";
	return (path);
}


// this function checks if a location field is empty or not
// if empty it fills it with the server field
void workingLocation::checkLocation(server *server)
{
	// if (serverlocation->get_client_max_body_size() == -1)
	// 	serverlocation->set_client_max_body_size(server->get_client_max_body_size());
	// if (serverlocation->get_root() == std::string(""))
	// 	serverlocation->set_root(server->get_root());
	// if (serverlocation->get_index().size() == 0)
	// 	serverlocation->fill_index(server->get_index(), 0);
	// if (serverlocation->get_methods().size() == 0)
	// 	serverlocation->fill_allowed_methods(server->get_allowed_methods(), 0);
}

// this function checks to be geted is already in uploads or not
