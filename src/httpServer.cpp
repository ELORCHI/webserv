#include "httpServer.hpp"
#include <netinet/in.h>
#include <sys/event.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <utility>


void send_file_to_socket(int fd, char *path)
{
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open())
		return;
	char buffer[4096];
	while (!file.eof())
	{
		file.read(buffer, 4096);
		int size = file.gcount();
		send(fd, buffer, size, 0);
	}
	file.close();
}

size_t file_size(std::string path)
{
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open())
		return 0;
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.close();
	return size;
}

bool httpServer::doesHttpRequestBelongs(request *rq)
{
    bool doesRequestHostBelong = false;
    bool doesRequestPortBelong = false;
    for (int i = 0; i < server_parsed_data.get_name_size(); i++)
    {
        if (server_parsed_data.get_name(i) == rq->getHost())
        {
            doesRequestHostBelong = true;
            std::cout << "Host: " << rq->getHost() << " belongs to server" << std::endl;
            std::cout << "server host: " << server_parsed_data.get_name(i) << std::endl;
            std::cout << "server port: " << this->listenServerPort << std::endl;
        }
   // std::cout << "request port: " << rq->getPort() << std::endl;
    }
    if (server_parsed_data.get_listen_port() == rq->getPort())
    {
        doesRequestPortBelong = true;
        std::cout << "Port: " << rq->getPort() << " belongs to server" << std::endl;
    }
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
    int wahd = 1;
    if (setsockopt(sd->listenServerFd, SOL_SOCKET, SO_REUSEADDR, &wahd, sizeof(wahd)) == -1)
    {
        std::cerr << "setsockopt failed" << std::endl;
        exit(1);
    }
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

httpServer::httpServer(server server_parsed_data,  bool is_shared_port, socket_data *sd, int KqueueFd)
{
    listenServerFd = INVALID_SOCKET;
    listenServerPort = server_parsed_data.get_listen_port(); 
    canRun = false;
    this->serverKqFd = KqueueFd;
    canRun = false;
    // listenServerFd = socket(AF_INET, SOCK_STREAM, 0);
    this->server_parsed_data = server_parsed_data;
        // std::cerr << is_shared_port << std::endl;
    try {
        if (!is_shared_port) 
            sd = create_listening_socket(listenServerPort, server_parsed_data.get_listen_host());
        // if ((serverKqFd = kqueue()) == -1)
        //     throw MyException("failure at creating the kernel queue");
    }
    catch (std::exception &e)
    {
        // std::cerr << e.what() << std::endl÷;
        std::cerr << e.what() << std::endl;
        exit(420);
    }
    listenServerFd = (*sd).listenServerFd;
    listeningServAddr = (*sd).listeningServAddr;
    //set server listening socket as non blockin
    fcntl(listenServerFd, F_SETFL, O_NONBLOCK);
    //add event read event to kqueue
    struct kevent _kEvent;
    EV_SET(&_kEvent, listenServerFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(serverKqFd, &_kEvent, 1, NULL, 0, NULL);
    // std::cout << "daaaah" << std::endl;
    canRun = true;
	run_once = false;

    //return true;
}


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
    EV_SET(&kEv, clFd, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, NULL);
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
    {
        clientmap.erase(c->getClientFd());
    }
    delete c;
}

void httpServer::resetClient(client *c)
{
    struct kevent kEv;

    if (!c)
        return ;
    EV_SET(&kEv, c->getClientFd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
    EV_SET(&kEv, c->getClientFd(), EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, NULL);
    kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);

   // c->set_is_connected(false);
    c->set_sendin_status(false);
    c->set_reading_status(false);
    
}

//will read from client and write to file then get the headers store then into a buffer
// set the headers buffer then store the rest of the body in a file 
void httpServer::read_from_client(client *c, long data_length)
{
    if (!c)
        return ;
	// static long long size = 0;
    // if (!c || request::is_request_complete(c->getHeadersBuffer(), c->getBodyFile()))
	// {
	// 	printf("wesh the pointer is: %p\n", c);
	// 	c->set_reading_status(true);
	// 	//disconnectClient(c, true);
    //     return ; 
	// }

    char *c_buffer = new char[data_length];
	// std::cerr << "DBG_00" << std::endl;
    int bytesRead = recv(c->getClientFd(), c_buffer, data_length, 0);
    // std::cout << c_buffer << std::endl;
    

	if (bytesRead <= 0)
    {
        if ((c->isConnectionType() && c->getConnectionType() == "close") || (!c->isConnectionType()))
        {
            disconnectClient(c, true);
        }
        else if (c->isConnectionType() && c->getConnectionType() == "keep-alive")
        {
            std::cerr << "DBG_01" << std::endl;
            disconnectClient(c, false);
        } 
    } 
	else
    {

        int k = c->get_pr().start_parsing(c_buffer, bytesRead);
        if (k)
            c->set_reading_status(true);
        std::cout << k << std::endl;
    }
	
    delete[] c_buffer;
}

void httpServer::write_to_client(client *cl, long data_length, std::string response_buffer)
{
    if (!cl)
        return ;
    int bytes_sent = 0; // number of bytes sent as returned by the send sys call
    int remaining_bytes = 0; // bytes that remain to be sent
    int attempt_to_send_bytes = 0; // number of bytes to be sent in the current send attempt

    remaining_bytes = response_buffer.size() - cl->get_send_offset();
    attempt_to_send_bytes = remaining_bytes > data_length ? data_length : remaining_bytes;
    bytes_sent = send(cl->getClientFd(), response_buffer.c_str() + cl->get_send_offset(), attempt_to_send_bytes, 0);
    if (bytes_sent == -1)
    {
        disconnectClient(cl, true);
        return ;
    }
    cl->set_send_offset(cl->get_send_offset() + bytes_sent);
    if (cl->get_send_offset() >= response_buffer.size())
    {
        cl->set_sendin_status(true);
    }
}

void httpServer::run(int num_events, struct kevent *_eventList)
{
    //int num_events = 0;
    client *cl;
    struct kevent kEv;
    bool run_once = false;

    if (canRun)
    {
        //get all the triggered events
        if (num_events <= 0)
		{
            return ;
		}
        for (int i = 0; i < num_events; i++)
        {
            if (_eventList[i].ident == listenServerFd) //a client is waiting to connect
			{
                acceptConnection();
			}
            else //a client fd triggered an event
            {
                if (clientmap.find(_eventList[i].ident) == clientmap.end())
				{
                    cl = NULL;
				}
                else
				{
                    cl = clientmap[_eventList[i].ident];
				}
                if (cl == NULL)
                {	
                    // EV_SET(&kEv, _eventList[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    // kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                    // EV_SET(&kEv, _eventList[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                    // kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                    // close(_eventList[i].ident);
					// std::cout << "cl is null" << std::endl;              
                    continue ;
                }

                // if (_eventList[i].flags & EV_EOF) {
				// 	// std::cout << "yo am i here" << std::endl;
                //     disconnectClient(cl, true);
                //     continue;
                // }
                if (_eventList[i].filter == EVFILT_READ && cl)
                { 
					read_from_client(cl, _eventList[i].data);
                    std::cerr << "cl: " << cl << std::endl;
                    if (cl && cl->is_reading_complete())
                    {
                        server *spd = getServerParsedData();
                        // std::cout << "testiiiiiiin" << std::endl;
                        // std::cout << spd->get_upload_path() << std::endl;
                        request *r = new request(cl->get_pr(), spd);
                        // if (doesHttpRequestBelongs(r))
                        //     cl->setRequest(r);
                        // else
                        // {
                        //     delete r;
                        //     disconnectClient(cl, true);
                        //     continue;
                        // }
                        cl->setRequest(r);
                        // std::cout << "req complete" << std::endl;
                        EV_SET(&kEv, _eventList[i].ident, EVFILT_READ, EV_DISABLE, 0, 0, NULL);
                        kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                        EV_SET(&kEv, _eventList[i].ident, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);
                        kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);

                        //keep alive header
						// disconnectClient(cl, true);	
                    }
	            }
                else if (_eventList[i].filter == EVFILT_WRITE && cl)
                {
                    if (cl->is_reading_complete())
					{
						std::string rep = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 37\r\n\r\n<html><body><h2>ok</h2></body></html>";
						// int s = send(cl->getClientFd(), rep.c_str(), rep.length(), 0);
                        //responseHandler *rh = NULL;
                        if (cl->is_sending_to_complete() == false)
                        {
                            run_once = true;
                            // std::cout << "coco: " << cl->getReadyRequest()->get_request_parsing_data().get_http_method() << std::endl;
                            responseHandler *rh = getResponse(cl);
                            std::cout <<  rh->getBuffer() << std::endl;
                           write_to_client(cl,  _eventList[i].data, rh->getBuffer());
                        }
                        // if (cl->is_sending_to_complete())
                        // {
                        //     std::cout << "hold on bruh" << std::endl;
                        //     disconnectClient(cl, true);
                        // }
                       // if (rh)
						// std::cout << "request complete" << std::endl;
                        // EV_SET(&kEv, _eventList[i].ident, EVFILT_READ, EV_ENABLE, 0, 0, NULL);
                        // kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                        // EV_SET(&kEv, _eventList[i].ident, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
                        // kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                        // cl->set_reading_status(false);
                        // if (cl->get_pr().get_http_headers().count("Keep-Alive") > 0)
                        // {
                        //     //get timeout and from keep-alive string
                        //     std::string s = cl->get_pr().get_http_headers()["Keep-Alive"];
                        //     // std::string con = cl->get_pr().get_http_headers()["Connection"];
                        //     cl->setKeepAliveInfo(s);
                        //     // cl->setConnectionType(con);

                        // }
                        if (cl && cl->is_sending_to_complete() && cl->get_pr().get_http_headers().count("Connection") > 0)
                        {
                            std::string con = cl->get_pr().get_http_headers()["Connection"];
                            cl->setConnectionType(con);
                            if (con == "close")
                            {
                                disconnectClient(cl, true);
                            }
                            else if(con == "keep-alive")
                            {
                                std::cerr << "DBG_02" << std::endl;
                                //resetClient(cl);
                            }

                        }
                        else if (cl && cl->is_sending_to_complete() && cl->get_pr().get_http_headers().count("Connection") == 0)
                        {
                            disconnectClient(cl, true);
                        }
    					// disconnectClient(cl, true);
                        // else
                        // {
                        // }
                        
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

int httpServer::getServerFd()
{
    return (this->listenServerFd);
}