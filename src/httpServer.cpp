#include "httpServer.hpp"
#include <netinet/in.h>
#include <sys/event.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <utility>



bool httpServer::doesHttpRequestBelongs(request *rq)
{
    bool doesRequestHostBelong = false;
    bool doesRequestPortBelong = false;
    for (int i = 0; i < server_parsed_data.get_name_size(); i++)
    {
        if (server_parsed_data.get_name(i) == rq->getHost())
            doesRequestHostBelong = true;
    }
    if (server_parsed_data.get_listen_port() == rq->getPort())
        doesRequestPortBelong = true;
    if (doesRequestHostBelong && doesRequestPortBelong)
        return true;
    return false;
}

std::set<int> httpServer::getRepeatedPorts(std::vector<server> parsed_servers_data)
{
    std::vector<int> ports;
    std::set<int> repeatedPorts;

    for (int i = 0; i < parsed_servers_data.size(); i++)
        ports.push_back(parsed_servers_data[i].get_listen_port());
    
    for (int i = 0; i < parsed_servers_data.size(); i++)
    {
        if (std::count(ports.begin(), ports.end(), parsed_servers_data[i].get_listen_port()) > 1)
        {
            repeatedPorts.insert(parsed_servers_data[i].get_listen_port());
            // std::cout << parsed_servers_data[i].get_listen_port() << std::endl;
        }
    }
    return (repeatedPorts);
}

socket_data *httpServer::create_listening_socket(int port, std::string host)
{
    socket_data *sd = new socket_data;
    if (host == "localhost")
        host = "127.0.0.1";
    sd->listenServerPort = port;
    memset(&sd->listeningServAddr, 0, sizeof(sd->listeningServAddr));
    sd->listeningServAddr.sin_family = AF_INET; 
    sd->listeningServAddr.sin_port = htons(port); //The htons function takes a 16-bit number in host byte order and returns a 16-bit number in network byte order used in TCP/IP networks
    sd->listeningServAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // inet_pton(AF_INET, host.c_str(), &(sd->listeningServAddr.sin_addr));
    sd->listenServerFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd->listenServerFd == INVALID_SOCKET)
        throw MyException("failed at creating the server socket!");
    int bind_r = bind(sd->listenServerFd, (struct sockaddr*)&(sd->listeningServAddr), sizeof(sd->listeningServAddr));
	
	std::cout << "port is: " << port << " host is: " << host <<  std::endl;
    if (bind_r != 0)
        throw MyException("binding address to socket failed!");
    if (listen(sd->listenServerFd, 100) != 0) 
        throw MyException("failed to put socket in listening state!"); 
    return sd;
}

int	httpServer::parsing_conf(int ac, char **av,parse_config *conf)
{
	// parse_server_config *conf = new parse_server_config;
	// parse_config conf;

	if (ac != 2)
	{
		std::cout << "Usage: ./parse_confile <path_to_config_file>" << std::endl;
		return (0);
	}
	std::ifstream file(av[1]);
	if (!file.is_open())
	{
		std::cout << "Error: file " << av[1] << " not found" << std::endl;
		return (0);
	}
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(file, line))
		lines.push_back(line);
	file.close();
	conf->set_lines(lines);
	// conf.read_lines();
	try
	{
		conf->start_parsing();
	}
	catch (std::runtime_error &e)
	{
		std::cout << e.what() << std::endl;
		return (0);
	}
	// conf->read_server();
	return (1);
}

// httpServer::httpServer(int port)
// {
//     listenServerFd = INVALID_SOCKET;
//     listenServerPort = port; 
//     canRun = false;
//     serverKqFd = -1;
//     memset(&listeningServAddr, 0, sizeof listeningServAddr);
//     listeningServAddr.sin_family = AF_INET; //IP v4 family
//     listeningServAddr.sin_port = htons(listenServerPort); //The htons function takes a 16-bit number in host byte order and returns a 16-bit number in network byte order used in TCP/IP networks
//     listeningServAddr.sin_addr.s_addr = htonl(INADDR_ANY); //accept all connections aka set address to 0.0.0.0
// }

httpServer::httpServer(server server_parsed_data,  bool is_shared_port, socket_data *sd)
{
    listenServerFd = INVALID_SOCKET;
    listenServerPort = server_parsed_data.get_listen_port(); 
    canRun = false;
    serverKqFd = -1;
    canRun = false;
    // listenServerFd = socket(AF_INET, SOCK_STREAM, 0);
    this->server_parsed_data = server_parsed_data;
        // std::cerr << is_shared_port << std::endl;
    try {
        if (!is_shared_port) 
            sd = create_listening_socket(listenServerPort, server_parsed_data.get_listen_host());
        if ((serverKqFd = kqueue()) == -1)
            throw MyException("failure at creating the kernel queue");
    }
    catch (std::exception &e)
    {
        // std::cerr << e.what() << std::endl÷;
        std::cerr << e.what() << std::endl;
    }
    listenServerFd = (*sd).listenServerFd;
    listeningServAddr = (*sd).listeningServAddr;
    //set server listening socket as non blockin
    fcntl(listenServerFd, F_SETFL, O_NONBLOCK);

    //add event read event to kqueue
    struct kevent _kEvent;
    EV_SET(&_kEvent, listenServerFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(serverKqFd, &_kEvent, 1, NULL, 0, NULL);
    canRun = true;
	run_once = 0;
    //return true;
}

// bool httpServer::start() {
//     canRun = false;
//     listenServerFd = socket(AF_INET, SOCK_STREAM, 0);
//     socket_data sd;
//     try {
//         if (listenServerFd == INVALID_SOCKET)
//             throw MyException("failed at creating the server socket!");
//         int bind_r = bind(listenServerFd, (struct sockaddr*)&listeningServAddr, sizeof(listeningServAddr));
//         if (bind_r != 0)
//         {
//             throw MyException("binding address to socket failed!");
//         }
//         if (listen(listenServerFd, 100) != 0) {
//             throw MyException("failed to put socket in listening state!");
//         }
//         sd = create_listening_socket(listenServerPort);
//         if ((serverKqFd = kqueue()) == -1)
//             throw MyException("failure at creating the kernel queue");
        
//     }
//     catch (std::exception &e)
//     {
//         std::cerr << e.what() << std::endl;
//         return false;
//     }
//     listenServerFd = sd.listenServerFd;
//     listeningServAddr = sd.listeningServAddr;
//     //set server listening socket as non blockin
//     fcntl(listenServerFd, F_SETFL, O_NONBLOCK);

//     //add event read event to kqueue
//     struct kevent _kEvent;
//     EV_SET(&_kEvent, listenServerFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
//     kevent(serverKqFd, &_kEvent, 1, NULL, 0, NULL);
//     canRun = true;
//     return true;
// }


void httpServer::stop()
{
    canRun = false;
    if (listenServerFd != INVALID_SOCKET)
    {
        close(listenServerFd);
        listenServerFd = INVALID_SOCKET;
    }

    //remove server socket event from kqueue
    struct kevent _kEv;
    EV_SET(&_kEv, listenServerFd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(serverKqFd, &_kEv, 1, NULL, 0, NULL);

    if (serverKqFd != -1)
    {
        close(serverKqFd);
        serverKqFd = -1;
    }
}

void httpServer::acceptConnection()
{
    struct sockaddr_in clientAddr;
    int clientAddrlen = sizeof(clientAddr);
    int clFd = INVALID_SOCKET;

    clFd = accept(listenServerFd, (sockaddr*)&clientAddr, (socklen_t*)&clientAddrlen);
    if (clFd == INVALID_SOCKET)
        return ;
    fcntl(clFd, F_SETFL, O_NONBLOCK);
    client *c = new client(clFd, clientAddr);

    struct kevent kEv;
    EV_SET(&kEv, clFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
    EV_SET(&kEv, clFd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
    clientmap[clFd] = c;
    //test block
    // send(c->getClientFd(), "bruh", 4, 0);
    //test block
}

void httpServer::disconnectClient(client *c, bool is_delete)
{
    struct kevent kEv;

    if (!c)
        return ;
    EV_SET(&kEv, c->getClientFd(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
    EV_SET(&kEv, c->getClientFd(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
    close(c->getClientFd());
    if (is_delete)
        clientmap.erase(c->getClientFd());
    delete c;
}

//will read from client and write to file then get the headers store then into a buffer
// set the headers buffer then store the rest of the body in a file 
void httpServer::read_from_client(client *c, long data_length)
{
    if (!c || c->is_reading_complete() == true)
        return ; 
    char *c_buffer = new char[data_length];
    int bytesRead = recv(c->getClientFd(), c_buffer, data_length, 0);
    if (bytesRead <= 0)
        disconnectClient(c, true);
    else
    {
		if (!request::is_requestHeaderComplete(c->getHeadersBuffer()))
		{
    		c->appendToHeadersBuffer(c_buffer);
			//std::cout << c->getHeadersBuffer() << std::endl;
		}
		if (request::is_requestHeaderComplete(c->getHeadersBuffer()))
		{
			std::string s = c->getHeadersBuffer();
			if (!run_once && s.substr(s.find("\r\n\r\n")+4).size())
			{
				// if (s.find("\r\n") != std::string::npos)
				std::cout << c->getHeadersBuffer() << std::endl;
				std::string ss = s.substr(s.find("\r\n\r\n") + 4); 
				c->appendToReadBodyFile(ss.c_str());
				// std::cout << "foo: " << ss << std::endl;

				// std::string st =  s.substr(s.find(" 3") + 2);
				// for (int i = 0; i < s.size(); i++)
					// std::cout << (int)s[i] << std::endl;
///				std::cout << "Made it\n";
				run_once = 1;
			}
			else
			{
				// std::cout << c_buffer << std::endl;
				c->appendToReadBodyFile(c_buffer);
			}
			// std::cout << "omega: " << c_buffer << std::endl;
		}
		// c->appendToReadTmpFile(c_buffer);
    }
	std::fstream &body = c->getBodyFile();
	//count the number of characters in body
	body.seekg(0, std::ios::end);
	int body_length = body.tellg();
	body.seekg(0, std::ios::beg);
    delete[] c_buffer;
}

void httpServer::run()
{
    int num_events = 0;
    client *cl;
    struct kevent kEv;

    if (canRun)
    {
        //get all the triggered events
        num_events = kevent(serverKqFd, NULL, 0, _eventList, MAX_EVENTS, NULL);
        if (num_events <= 0)
		{
			// std::cout << "haaaah" << std::endl;
            return ;
		}
        for (int i = 0; i < num_events; i++)
        {
			// std::cout << "hoooooo" << std::endl;
            if (_eventList[i].ident == listenServerFd) //a client is waiting to connect
			{
                acceptConnection();
				// std::cout << "fuck" << std::endl;
			}
            else //a client fd triggered an event
            {
                if (clientmap.find(_eventList[i].ident) == clientmap.end())
				{
                    cl = NULL;
				}
                else
                    cl = clientmap[_eventList[i].ident];
                if (cl == NULL)
                {
                    EV_SET(&kEv, _eventList[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                    EV_SET(&kEv, _eventList[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                    kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                    close(_eventList[i].ident);                    
                    continue ;
                }

                if (_eventList[i].flags & EV_EOF) {
                    disconnectClient(cl, true);
                    continue;
                }
                if (_eventList[i].filter == EVFILT_READ)
                {
                    //read data from client socket
                    // std::cout << "smth cooking" << std::endl;
					// std::cout << "bro" << std::endl;	
                    read_from_client(cl, _eventList[i].data);

						// std::cout << cl->getHeadersBuffer() << std::endl;
					// disconnectClient(cl, true);
                    //have kqueue disable tracking read events and enable write events
                    // if (request::is_requestHeaderComplete(cl->getHeadersBuffer()))
					// {
						// if (request)
						// char buffer[40000];
						// std::ifstream fs;
						// fs.open("index.html");
						// fs.read(buffer,40000);
						//fs.read()

						// std::string rep = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 37\r\n\r\n<html><body><h2>yo booooooy</h2></body></html>";
						// send(cl->getClientFd(), rep.c_str(), 102, 0);
						// disconnectClient(cl, true);
					// }
                    // if (cl->is_reading_complete())
                    // {
						//	disconnectClient(cl, true);
                        //do smth with the data recieved
                        // request
                        // request *rq = new request(cl->getReadBuffer(), listenServerPort, &server_parsed_data);
                        // if (doesHttpRequestBelongs(rq))
                        //     cl->setRequest(rq);
                        // else
                        //     delete rq;
                        // EV_SET(&kEv, _eventList[i].ident, EVFILT_READ, EV_DISABLE, 0, 0, NULL);
                        // kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                        // EV_SET(&kEv, _eventList[i].ident, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);
                        // kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                    // }
                }
                else if (_eventList[i].filter == EVFILT_WRITE)
                {
					if (request::is_requestHeaderComplete(cl->getHeadersBuffer()))
					{
						std::string rep = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 37\r\n\r\n<html><body><h2>yo booooooy wassup</h2></body></html>";
						send(cl->getClientFd(), rep.c_str(), 102, 0);
						disconnectClient(cl, true);	
					}
                    // if (cl->isThereARequestReady() == true)
					// {
						// std::fstream fs("test.txt", std::fstream::in | std::fstream::out);
						// fs << cl->getReadBuffer();
						//std::cout << cl->getReadBuffer();
						
						// std::cout << "bro" << std::endl;
						// disconnectClient(cl, true);
					// }
                    // if (cl->isThereARequestReady() == true)
                    // {
                    //     //response *res = new response(cl)
					// 	if (cl->resp) // if we already geneerated a response and we didn't finish sending it
					// 	{
					// 		if ((cl->resp.send()) == 0) // if we done  sending
					// 			disconnectClient(cl, true);
					// 	}
					// 	else {
					// 		cl->resp = new response(cl);
					// 		if ((cl->resp.send()) == 0) // if we done  sending
					// 			disconnectClient(cl, true);
					// 	}

                    
					// }
                    // have kqueue disable tracking write events and enable read events after data sending is done
                    // EV_SET(&kEv, _eventList[i].ident, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
                    // kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                    // EV_SET(&kEv, _eventList[i].ident, EVFILT_READ, EV_ENABLE, 0, 0, NULL);
                    // kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                }
                //TODO: a client fd can be either ready for read or write or at the end of file
                //de
            }
        }
    }
}

