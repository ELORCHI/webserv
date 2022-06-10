#include "../includes/response.hpp"


// if the hanlde method returns 1 the request will be passed to next handler
// if the handle method returns 0 the request will be handled by the calling object


//// check this  if delete method caused a problem


response::response()
{

}

response::~response()
{

}

responseHandler::responseHandler(client _cl): cl(_cl)
{
	buffer = "";
	response_body = "";
	headers = "";
	statusLine = "";
	error = 0;
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


system_block_response::system_block_response(): responseHandler()
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
	if (isError(status))
		return (getDefaultError(status, info));
	else
		return (bodyMsg);
}

std::string getDefaultError(int status, Locator *info)
{
	std::string body = "";
	std::string path;
	if (status >= 400)
	{
		path = info->getWorkingLocation()->getDefaultError(status);
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
	if (status != NO_CONTENT)
		headers += "content-length: " + std::string(ss) + std::string("\n");
	headers += "Server: " + std::string("420 SERVER") + std::string("\n");
	headers += "content-type: " + info->getContentType() + std::string("\n");
	headers += "date: " + formatted_time() + std::string("\n");
	if (status == MOVED_PERMANENTLY)
	{
		int i = info->isredirection();
		if (i != -1)
			headers += "location: " + info->getWorkingLocation()->getRedirections()[i][1] + std::string("\n");
		else
			headers += "location: " + info->getResourceFullPath() + std::string("\n");
	}
	else if (status == CREATED_CODE)
		headers += "location: " + info->getResourceFullPath() + std::string("\n");
	std::map<std::string, std::string> headersmap =  info->getClient().getReadyRequest()->get_request_parsing_data().get_http_headers();
	std::map<std::string, std::string>::iterator it = headersmap.find("connection"); 
	if (it != headersmap.end())
		headers += "connection " + headersmap["connection"] + std::string("\n");
	headers += "\r\n";
	return headers;
}

std::string getResponseHeaders(int status, int body_size)
{
	std::string headers;
	std::stringstream s;
	std::string ss;
	s << body_size;
	s >> ss;
	if (status != NO_CONTENT)
		headers += "content-length: " + std::string(ss) + std::string("\n");
	headers += "connection: close\n";
	headers += "Server: " + std::string("420 SERVER") + std::string("\n");
	headers += "content-type: " + std::string("text/html") + std::string("\n");
	headers += "date: " + formatted_time() + std::string("\n");
	headers += "\r\n";
	return headers;
}

client responseHandler::getClient(void)
{
	return cl;
}

std::string Locator::getContentType(void)
{
	std::string res;
	size_t i = resourceFullPath.rfind('.', resourceFullPath.length());
	if (i != std::string::npos)
	{
	   res.substr(i + 1, resourceFullPath.length() - i);
	}
	else
		res = "";
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


void responseHandler::setClient(client &_cl)
{
	cl = _cl;
}

int system_block_response::handle()
{
	int err = 0;
	if (this->isMethodImplemented(cl.getReadyRequest()->get_request_parsing_data().get_http_method()))
		err = 1;
	else if (this->isHttpVersionSupported(cl.getReadyRequest()->get_request_parsing_data().get_http_version()) == 1)
		err = 1;
	else if (cl.getReadyRequest()->get_request_parsing_data().get_code_status() == 500)
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
	switch (error)
	{
		case HTTP_VERSION_NOT_SUPPORTED_CODE:
			response_body = getErroBody(error, HTTP_VERSION_NOT_SUPPORTED_RESPONSE_MSG, cl);
			statusLine = getResponseStatusLine(error, HTTP_VERSION_NOT_SUPPORTED_MSG);		
			headers = 	getResponseHeaders(error, response_body.size());
		case NOT_IMPLEMENTED_CODE:
			response_body = getErroBody(error, NOT_IMPLEMENTED_RESPONSE_MSG, cl);
			statusLine = getResponseStatusLine(error, NOT_IMPLEMENTED_MSG);		
			headers = 	getResponseHeaders(error, response_body.size());
		case INTERNAL_SERVER_ERROR_CODE:
			response_body = getErroBody(error, INTERNAL_SERVER_ERROR_RESPONSE_MSG, cl);
			statusLine = getResponseStatusLine(error, INTERNAL_SERVER_ERROR_MSG);		
			headers = 	getResponseHeaders(error, response_body.size());
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


location* workingLocation::searchLocation(std::vector<location> locations, std::string source)
{
	int max_match_length = 0;
	location *loc = new location();
	int	match;
	int size = locations.size();
	for (int i = 0; i < size; i++)
	{
		match = 0;
		for (int j = 0; j < locations[i].get_locations_path().size(); i++)
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
		return (NULL);
	return loc;
}

void workingLocation::setlocation(request *request)
{
	location *loc = this->searchLocation(request->get_server()->get_location(), request->get_request_parsing_data().get_http_path());
	if (loc == NULL)
		loc = this->defaultLocation(request->get_server());
	serverlocation = loc;
	setUpload(request->get_server()->get_upload_path());
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


Locator::Locator(client &_cl): responseHandler(_cl)
{
	// this->setClient(cl);
	Locate = new workingLocation;
	resourceFullPath = "";
	hasIndex = false;
	autoindex = false;
	hasIndex = false;
}

Locator::~Locator()
{
	delete Locate;
}

void Locator::setResourceFullPath()
{
	resourceFullPath = Locate->getLocation()->get_root() + cl.getReadyRequest()->get_request_parsing_data().get_http_path();
}

std::string Locator::getResourceFullPath(void)
{
	return resourceFullPath;
}


void Locator::checker(void)
{
	struct stat s;
	std::string path = cl.getReadyRequest()->get_request_parsing_data().get_http_path();
	setworkingLocation();
	setResourceFullPath();
	setAutoIndex();
	if (isCgi(path))
		resourceType = CG;
	else if (stat(path.c_str(), &s))
	{
		if (s.st_mode & S_IFDIR)
		{
			resourceType = DIRE;
			setIndex();
		}
		else
			resourceType = FI;
	}
	else
		resourceType = NO;
	if (*(path.rbegin()) == 92)
		endwithslash = true;
	else
		endwithslash = false;
}


// int Locator::HandleCGI()
// {
// 	error = 1;
// }

std::string Locator::getindexfile(void)
{
	std::string indexfile;
	std::vector<std::string> _indexs = getWorkingLocation()->getLocation()->get_index();
	int size = _indexs.size();
	struct stat s;

	for (int i = 0; i < size; i++)
	{
		indexfile = _indexs[i];
		if (stat(indexfile.c_str(), &s))
		{
			if (!(s.st_mode & S_IFDIR))
				break;
		}
	}
	return (indexfile);
}

void Locator::setworkingLocation(void)
{
	this->Locate->setlocation(cl.getReadyRequest());
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

std::string getErroBody(int erroCode, std::string definebody, client &cl)
{
	if (erroCode < 400)
		return (definebody);
	std::vector<std::vector<std::string> > defaultError = cl.getReadyRequest()->get_server()->get_error_pages();
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
	return (path); 
}

std::string Locator::readBody(std::string path)
{
	std::fstream in_file(path);
	std::string body = "";
	struct stat sb;

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

int Locator::isredirection()
{
	std::vector<std::vector<std::string> > redirections = getWorkingLocation()->getRedirections();
	int size = redirections.size();
	for (int i = 0; i < size; i++)
	{
		std::vector<std::string> it = redirections[i];
		if (it[0] == cl.getReadyRequest()->get_request_parsing_data().get_http_path())
			return (i);
	}
	return (-1);
}

int Locator::handle()
{
	error = 0;
	if (!isMethodAllowd(cl.getReadyRequest()->get_request_parsing_data().get_http_method()))
		error = NOT_ALLOWED_CODE;
	else if (cl.getReadyRequest()->get_request_parsing_data().get_code_status() == 400)
		error = RESPONSE_BAD_REQUEST;
	else if (isredirection() != -1)
		error = MOVED_PERMANENTLY;
		return (0);
	if (error != 0)
		buildresponse();
	return (1);
}


bool Locator::isCgi(std::string path)
{
	if (Locate->getCgi() == NULL)
		return (false);
	if (cl.getReadyRequest()->get_request_parsing_data().get_http_path().find_last_of(".php"))
		return true;
	return false;
}

void Locator::setResourceFullPath(std::string path)
{
	resourceFullPath = path;
}

void Locator::buildresponse()
{
	switch (error)
	{
	case RESPONSE_BAD_REQUEST:
		response_body = getResponseBody(RESPONSE_BAD_REQUEST, BAD_REQUEST_RESPONSE_MSG, this);
		statusLine = getResponseStatusLine(RESPONSE_BAD_REQUEST, BAD_REQUEST_MSG);
		headers = getResponseHeaders(RESPONSE_BAD_REQUEST, this, response_body.size());
	case NOT_ALLOWED_CODE:
		response_body = getResponseBody(NOT_ALLOWED_CODE, NOT_ALLOWED_RESPONSE_MSG, this);
		statusLine = getResponseStatusLine(NOT_ALLOWED_CODE, NOT_ALLOWED_MSG);
		headers = getResponseHeaders(NOT_ALLOWED_CODE, this, response_body.size());
	case MOVED_PERMANENTLY:
		response_body = getResponseBody(MOVED_PERMANENTLY, MOVED_PERMANENTLY_RESPONSE_MSG, this);
		statusLine = getResponseStatusLine(MOVED_PERMANENTLY, MOVED_PERMANENTLY_MSG);
		headers = getResponseHeaders(MOVED_PERMANENTLY, this, response_body.size());
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
	if (Locate->getLocation()->get_index().size() == 0)
		return (false);
	return (true);
}

GetHandler::GetHandler(Locator *_godFather): responseHandler(_godFather->getClient())
{
	godFather = _godFather;
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
	std::string path	= godFather->getResourceFullPath();
	std::string host	= cl.getReadyRequest()->get_server()->get_listen_host();
	int port			= cl.getReadyRequest()->get_server()->get_listen_port();

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
	if (Locate->getLocation()->get_index().size() > 0)
		hasIndex = true;
	else
		hasIndex = false;
}

void GetHandler::setGodFather(Locator *_godFather)
{
	this->godFather = _godFather;
}

int GetHandler::handle()
{
	if (godFather->getResourceType() == NO)
		error = NOT_FOUND_CODE;
	else if (godFather->getResourceType() == FI)
			handleFiles();
	else
		HandleDir();
	buildresponse();
	return (1);
};

int GetHandler::HandleDir(void)
{
	std::string newpath;

	if (!godFather->gedEnd())
	{
		newpath = godFather->getResourceFullPath() + "/";
		godFather->setResourceFullPath(newpath);
		error = MOVED_PERMANENTLY;
	}
	else if (!godFather->getIndex())
	{
		if (godFather->getAutoIndex())
			error = AUTOINDEX_CODE;
		else
			error = FORBIDDEN_CODE;
	}
	else
	{
		newpath = godFather->getResourceFullPath() + godFather->getindexfile();
		godFather->setResourceFullPath(newpath);
		handleFiles();
	}
	return (1);
}

int GetHandler::handleFiles()
{
	if (godFather->isCgi(godFather->getResourceFullPath()))
		godFather->HandleCGI();
	else
		error = 200;
	return (1);
}

void GetHandler::buildresponse()
{
	switch (error)
	{
	case AUTOINDEX_CODE:
		response_body = getResponseBody(200, setAutoindexResponse(), godFather);
		statusLine = getResponseStatusLine(AUTOINDEX_CODE, OK_MSG);
		headers = getResponseHeaders(200, godFather, response_body.size());
	case MOVED_PERMANENTLY:
		response_body = getResponseBody(MOVED_PERMANENTLY, MOVED_PERMANENTLY_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(MOVED_PERMANENTLY, MOVED_PERMANENTLY_MSG);
		headers = getResponseHeaders(MOVED_PERMANENTLY, godFather, response_body.size());
	case 200:
		response_body = getResponseBody(200, godFather->readBody(godFather->getResourceFullPath()), godFather);
		statusLine = getResponseStatusLine(200, OK_MSG);
		headers = getResponseHeaders(200, godFather, response_body.size());
	case FORBIDDEN_CODE:
		response_body = getResponseBody(FORBIDDEN_CODE, FORBIDDEN_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(FORBIDDEN_CODE, FORBIDDEN_MSG);
		headers = getResponseHeaders(FORBIDDEN_CODE, godFather, response_body.size());
	case NOT_FOUND_CODE:
		response_body = getResponseBody(NOT_FOUND_CODE, NOT_FOUND_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(NOT_FOUND_CODE, FORBIDDEN_MSG);
		headers = getResponseHeaders(NOT_FOUND_CODE, godFather, response_body.size());
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
	if (godFather->getResourceType() == NO)
		error = NOT_FOUND_CODE;
	else if (godFather->getResourceType() == FI)
			handleFiles();
	else
		HandleDir();
	buildresponse();
	return (1);
};

int DeleteHandler::deleter(std::string path)
{
	if (godFather->getResourceType() == DIRE)
	{
		return (rmtree(path.c_str()));	
	}
	else if (remove(path.c_str()) == 0)
		return (0);
	else
		return (1);
}

int DeleteHandler::handleFiles(void)
{
	if (godFather->isCgi(godFather->getResourceFullPath()))
		godFather->HandleCGI();
	else
	{
		if (deleter(godFather->getResourceFullPath()) == 0)
			error = NO_CONTENT;
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
	if (!godFather->gedEnd())
		error = CONFLICT;
	else if (godFather->isCgi(godFather->getResourceFullPath()))
	{
		if (godFather->getIndex())
			godFather->HandleCGI();
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
	switch (error)
	{
	case NO_CONTENT:
		statusLine = getResponseStatusLine(NO_CONTENT, NO_CONTENT_MSG);
		headers = getResponseHeaders(NO_CONTENT, godFather, 0);
	case FORBIDDEN_CODE:
		response_body = getResponseBody(FORBIDDEN_CODE, FORBIDDEN_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(FORBIDDEN_CODE, FORBIDDEN_MSG);
		headers = getResponseHeaders(FORBIDDEN_CODE, godFather, response_body.size());
	case INTERNAL_SERVER_ERROR_CODE:
		response_body = getResponseBody(INTERNAL_SERVER_ERROR_CODE, INTERNAL_SERVER_ERROR_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(INTERNAL_SERVER_ERROR_CODE, INTERNAL_SERVER_ERROR_MSG);
		headers = getResponseHeaders(INTERNAL_SERVER_ERROR_CODE, godFather, response_body.size());
	case CONFLICT:
		response_body = getResponseBody(CONFLICT, CONFLICT_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(CONFLICT, CONFLICT_MSG);
		headers = getResponseHeaders(CONFLICT, godFather, response_body.size());
	case NOT_FOUND_CODE:
		response_body = getResponseBody(NOT_FOUND_CODE, NOT_FOUND_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(NOT_FOUND_CODE, FORBIDDEN_MSG);
		headers = getResponseHeaders(NOT_FOUND_CODE, godFather, response_body.size());
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


//======= needs to be implemented =====

int PostHandler::creator(std::string path)
{
	std::string newf = godFather->getWorkingLocation()->getLocation()->get_root();
	if ((newf.find_last_of("/") != newf.size() - 1) && (godFather->getWorkingLocation()->getUpload().find_first_of("/") != 0))
		newf += "/";
	else if ((newf.find_last_of("/") != newf.size() - 1) && (godFather->getWorkingLocation()->getUpload().find_first_of("/") != 0))
		newf.pop_back();
	newf += godFather->getWorkingLocation()->getUpload();
	if (newf.find_last_of("/") != newf.size() - 1)
		newf += "/";
	newf += getClient().getReadyRequest()->get_request_parsing_data().get_path_body();
	
	int new_fd = open(newf.c_str(), O_RDWR | O_CREAT | O_APPEND, 0644);
	int old_fd = open(getClient().getReadyRequest()->get_request_parsing_data().get_path_body().c_str(), O_RDONLY, 0644);
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
	close(new_fd);
	close(old_fd);
	unlink(getClient().getReadyRequest()->get_request_parsing_data().get_path_body().c_str());
	remove(getClient().getReadyRequest()->get_request_parsing_data().get_path_body().c_str());
	return (1);
}

int PostHandler::handle()
{
	if (supportAppload() && creator(godFather->getResourceFullPath()))
		error = CREATED_CODE;
	else
	{
		if (godFather->getResourceType() == FI)
			handleFiles();
		else if (godFather->getResourceType() == DIRE)
			HandleDir();
		else
			error = NOT_FOUND_CODE;
	}
	return (1);
}

int PostHandler::handleFiles(void)
{
	if (godFather->isCgi(godFather->getResourceFullPath()))
		godFather->HandleCGI();
	else
		error = FORBIDDEN_CODE;
	return (1);
}

int PostHandler::HandleDir(void)
{
	std::string newpath;
	if (godFather->gedEnd())
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
		//append slach
		newpath = godFather->getResourceFullPath() + "/";
		godFather->setResourceFullPath(newpath);
		error = MOVED_PERMANENTLY;
	}
	return (1);
}

void PostHandler::buildresponse()
{
	switch (error)
	{
	case CREATED_CODE:
		response_body = getResponseBody(CREATED_CODE, CREATED_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(CREATED_CODE, CREATED_MSG);
		headers = getResponseHeaders(CREATED_CODE, godFather, response_body.size());
	case NOT_FOUND_CODE:
		response_body = getResponseBody(NOT_FOUND_CODE, NOT_FOUND_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(NOT_FOUND_CODE, FORBIDDEN_MSG);
		headers = getResponseHeaders(NOT_FOUND_CODE, godFather, response_body.size());
	case FORBIDDEN_CODE:
		response_body = getResponseBody(FORBIDDEN_CODE, FORBIDDEN_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(FORBIDDEN_CODE, FORBIDDEN_MSG);
		headers = getResponseHeaders(FORBIDDEN_CODE, godFather, response_body.size());
	case MOVED_PERMANENTLY:
		response_body = getResponseBody(MOVED_PERMANENTLY, MOVED_PERMANENTLY_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(MOVED_PERMANENTLY, MOVED_PERMANENTLY_MSG);
		headers = getResponseHeaders(MOVED_PERMANENTLY, godFather, response_body.size());
	case INTERNAL_SERVER_ERROR_CODE:
		response_body = getResponseBody(INTERNAL_SERVER_ERROR_CODE, INTERNAL_SERVER_ERROR_RESPONSE_MSG, godFather);
		statusLine = getResponseStatusLine(INTERNAL_SERVER_ERROR_CODE, INTERNAL_SERVER_ERROR_MSG);
		headers = getResponseHeaders(INTERNAL_SERVER_ERROR_CODE, godFather, response_body.size());
	default:
		break;
	}
}


responseHandler *getResponse(client  &cl)
{
	responseHandler *systemResponse = new system_block_response();
	Locator *locationHandler = new Locator(cl);
	std::string method = cl.getReadyRequest()->get_request_parsing_data().get_http_method();
	if (systemResponse->handle())
	{
		delete locationHandler;
		return (systemResponse);
	}
	else
		locationHandler->checker();
	if (locationHandler->handle())
	{
		delete systemResponse;
		return locationHandler;
	}
	if (method == std::string("POST"))
	{
		responseHandler *_postHandler = new PostHandler(locationHandler);
		_postHandler->handle();
		delete locationHandler;
		delete systemResponse;
		return (_postHandler);
	}
	if (method == std::string("GET"))
	{
		responseHandler *_getHandler = new GetHandler(locationHandler);
		_getHandler->handle();
		delete locationHandler;
		delete systemResponse;
		return (_getHandler);
	}
	else
	{
		responseHandler *_deleteHandler = new DeleteHandler(locationHandler);
		_deleteHandler->handle();
		delete locationHandler;
		delete systemResponse;
		return _deleteHandler;
	}
	return (0);
}


