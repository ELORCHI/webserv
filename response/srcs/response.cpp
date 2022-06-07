#include "../includes/response.hpp"


// if the hanlde method returns 1 the request will be passed to next handler
// if the handle method returns 0 the request will be handled by the calling object


void responseHandler::setResposeStatusLine(int status, std::string status_line)
{
	char *tmp;

	std::string line = HTTP_VERSION_1_1 + std::string(" ") + status_line + std::string("\n\r");
	buffer = strdup(line.c_str());
	bufferOffset = line.size() + 3;
}

void responseHandler::setResponseHeaders(void)
{
	// add default headers;
}


int system_block_response::handle()
{
	int err = 0;
	if (this->isMethodImplemented(cl.getReadyRequest()->get_request_parsing_data().get_http_method()) == 1)
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
			responseHandler::setResposeStatusLine(HTTP_VERSION_NOT_SUPPORTED_CODE, HTTP_VERSION_NOT_SUPPORTED_MSG);
			responseHandler::setResponseHeaders();//no header so just default ones
			responseHandler::setResponseBody(HTTP_VERSION_NOT_SUPPORTED_RESPONSE_MSG);
			break;
		case NOT_IMPLEMENTED_CODE:
			this->setResposeStatusLine(NOT_IMPLEMENTED_CODE, NOT_IMPLEMENTED_MSG);
			responseHandler::setResponseHeaders();// no header just default ones
			this->setResponseBody(NOT_IMPLEMENTED_RESPONSE_MSG);
		case INTERNAL_SERVER_ERROR_CODE:
			this->setResposeStatusLine(INTERNAL_SERVER_ERROR_CODE, INTERNAL_SERVER_ERROR_MSG);
			responseHandler::setResponseHeaders();//no header so just default ones
			this->setResponseBody(INTERNAL_SERVER_ERROR_RESPONSE_MSG);
			break;
		default:
			return;
	}
}



// each location in the server block has its own path
// we will match the path of the request with the path of each location and chose the location with the longest match
//Nginx begins by checking all prefix-based location matches.
//It checks each location against the complete request URI.
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
	loc->set_index(server->get_index());
	loc->set_autoindex(server->get_autoindex());
	loc->set_allowed_methods(server->get_allowed_methods());
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
// sets isAllowed to true if found in the location methods
// needs getters for allowed methods
// void Locator::setMethodAllowance(std::string method)
// {
// 	for (int i; i < workingLocation->get_allowd_methods().size; i++)
// 	{

// 	}
// }

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
	if (isCgi(path))
		resourceType = CG;
	else if (stat(path.c_str(), &s))
	{
		if (s.st_mode & S_IFDIR)
		{
			resourceType = DIR;
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
	std::vector<std::string> _indexs = getWorkingLocation()->getLocation()->get_indexs();
	int size = _indexs.size();
	struct stat s;

	for (int i = 0; i < size; i++)
	{
		indexfile = _indexs[i];
		// indexfile = needs update
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

int Locator::getResourceType(void)
{
	return (resourceType);
}

int Locator::handle()
{
	error = 0;
	if (!isMethodAllowd(cl.getReadyRequest()->get_request_parsing_data().get_http_method()))
		error = NOT_ALLOWED_CODE;
	else if (cl.getReadyRequest()->get_request_parsing_data().get_code_status() == 400)
		error = RESPONSE_BAD_REQUEST;
	else
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
		responseHandler::setResposeStatusLine(RESPONSE_BAD_REQUEST, BAD_REQUEST_MSG);
		responseHandler::setResponseHeaders();//no header so just default ones
		responseHandler::setResponseBody(HTTP_VERSION_NOT_SUPPORTED_RESPONSE_MSG);
	case NOT_ALLOWED_CODE:
		responseHandler::setResposeStatusLine(NOT_ALLOWED_CODE, NOT_ALLOWED_MSG);
		responseHandler::setResponseHeaders();//no header so just default ones
		responseHandler::setResponseBody(NOT_ALLOWED_RESPONSE_MSG);
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

GetHandler::GetHandler(Locator *_godFather): responseHandler()
{
	godFather = _godFather;
}

void	Locator::setIndex(void)
{
	if (Locate->getLocation()->get_indexs().size() > 0)
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
		error = MOVED_PERMANENTLY;
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
		responseHandler::setResposeStatusLine(AUTOINDEX_CODE, OK_MSG);
		responseHandler::setResponseHeaders();//
		this->setResponseBody(setAutoindexResponse().c_str());
	case MOVED_PERMANENTLY:
		responseHandler::setResposeStatusLine(MOVED_PERMANENTLY, MOVED_PERMANENTLY_MSG);
		responseHandler::setResponseHeaders();// set location header feild
		setResponseBody(MOVED_PERMANENTLY_RESPONSE_MSG);
	case 200:
		responseHandler::setResposeStatusLine(200, OK_MSG);
		responseHandler::setResponseHeaders();//
		this->setResponseBody(godFather->readBody(godFather->getResourceFullPath()));
	case FORBIDDEN_CODE:
		responseHandler::setResposeStatusLine(FORBIDDEN_CODE, FORBIDDEN_MSG);
		responseHandler::setResponseHeaders();//
		responseHandler::setResponseBody(FORBIDDEN_RESPONSE_MSG);
	case NOT_FOUND_CODE:
		responseHandler::setResposeStatusLine(NOT_FOUND_CODE, NOT_FOUND_MSG);
		responseHandler::setResponseHeaders();//
		responseHandler::setResponseBody(NOT_FOUND_RESPONSE_MSG);
	default:

		break;
	}
}

DeleteHandler::DeleteHandler(Locator *_godFather): responseHandler()
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
{	// add folders to be deleted too
	// should i check for file permisions if yes return 2;
	if (remove(path.c_str()) == 0)
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
		if (deleter(godFather->getResourceFullPath()) == 0) // you maight need to check deleter return 
			error = NO_CONTENT;
		else
			error = INTERNAL_SERVER_ERROR_CODE;
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
		//DELETE ALL DIR FILE
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
		responseHandler::setResposeStatusLine(NO_CONTENT, NO_CONTENT_MSG);
		this->setResponseHeaders();// no content-length
	case FORBIDDEN_CODE:
		responseHandler::setResposeStatusLine(FORBIDDEN_CODE, FORBIDDEN_MSG);
		responseHandler::setResponseHeaders();//
		responseHandler::setResponseBody(FORBIDDEN_RESPONSE_MSG);
	case INTERNAL_SERVER_ERROR_CODE:
		responseHandler::setResposeStatusLine(INTERNAL_SERVER_ERROR_CODE, INTERNAL_SERVER_ERROR_MSG);
		responseHandler::setResponseHeaders();//no header so just default ones
		responseHandler::setResponseBody(INTERNAL_SERVER_ERROR_RESPONSE_MSG);
	case CONFLICT:
		responseHandler::setResposeStatusLine(CONFLICT, CONFLICT_MSG);
		responseHandler::setResponseHeaders();
		this->setResponseBody(CONFLICT_RESPONSE_MSG);// describe why
	case NOT_FOUND_CODE:
		responseHandler::setResposeStatusLine(NOT_FOUND_CODE, NOT_FOUND_MSG);
		responseHandler::setResponseHeaders();//
		responseHandler::setResponseBody(NOT_FOUND_RESPONSE_MSG);
	default:
		break;
	}
}


PostHandler::PostHandler(Locator *_godFather): responseHandler()
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


// int PostHandler::creator(std::string path)
// {
// 	std::string loc = godFather->getWorkingLocation()->getUpload() + godFather->getResourceFullPath();
// 	std::ofstream filestream(path);
// 	std::ifstream 
// 	std::string line;

// 	while (std::getline(, line))
// 	{
// 		filestream << line;
// 	}
	// return (1) //if error
// }

int PostHandler::handle()
{
	if (supportAppload() && creator(godFather->getResourceFullPath()))
		error = CREATED_CODE;
	else
	{
		if (godFather->getResourceType() == FI)
			handleFiles();
		else if (godFather->getResourceType() == DIR)
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
		error = MOVED_PERMANENTLY;
	return (1);
}

void PostHandler::buildresponse()
{
	switch (error)
	{
	case CREATED_CODE:
		responseHandler::setResposeStatusLine(CREATED_CODE, CREATED_MSG);
		responseHandler::setResponseHeaders();//add new file location
		responseHandler::setResponseBody(NOT_FOUND_RESPONSE_MSG);
	case NOT_FOUND_CODE:
		responseHandler::setResposeStatusLine(NOT_FOUND_CODE, NOT_FOUND_MSG);
		responseHandler::setResponseHeaders();//
		responseHandler::setResponseBody(NOT_FOUND_RESPONSE_MSG);
	case FORBIDDEN_CODE:
		responseHandler::setResposeStatusLine(FORBIDDEN_CODE, FORBIDDEN_MSG);
		responseHandler::setResponseHeaders();//
		responseHandler::setResponseBody(FORBIDDEN_RESPONSE_MSG);
	case MOVED_PERMANENTLY:
		responseHandler::setResposeStatusLine(MOVED_PERMANENTLY, MOVED_PERMANENTLY_MSG);
		responseHandler::setResponseHeaders();// set location header feild
		setResponseBody(MOVED_PERMANENTLY_RESPONSE_MSG);
	case INTERNAL_SERVER_ERROR_CODE:
		responseHandler::setResposeStatusLine(INTERNAL_SERVER_ERROR_CODE, INTERNAL_SERVER_ERROR_MSG);
		responseHandler::setResponseHeaders();//no header so just default ones
		responseHandler::setResponseBody(INTERNAL_SERVER_ERROR_RESPONSE_MSG);
	default:
		break;
	}
}

