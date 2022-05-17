#include "server.hpp"
#include <netinet/in.h>
#include <sys/event.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <utility>

server::server(int port)
{
    listenServerFd = INVALID_SOCKET;
    listenServerPort = port; 
    canRun = false;
    serverKqFd = -1;
    memset(&listeningServAddr, 0, sizeof listeningServAddr);
    listeningServAddr.sin_family = AF_INET; //IP v4 family
    listeningServAddr.sin_port = htons(listenServerPort); //The htons function takes a 16-bit number in host byte order and returns a 16-bit number in network byte order used in TCP/IP networks
    listeningServAddr.sin_addr.s_addr = htonl(INADDR_ANY); //accept all connections aka set address to 0.0.0.0
}


bool server::start() {
    canRun = false;
    listenServerFd = socket(AF_INET, SOCK_STREAM, 0);

    try {
        if (listenServerFd == INVALID_SOCKET)
            throw MyException("failed at creating the server socket!");
        int bind_r = bind(listenServerFd, (struct sockaddr*)&listeningServAddr, sizeof(listeningServAddr));
        if (bind_r != 0)
        {
            throw MyException("binding address to socket failed!");
        }
        if (listen(listenServerFd, 100) != 0) {
            throw MyException("failed to put socket in listening state!");
        }
        if ((serverKqFd = kqueue()) == -1)
            throw MyException("failure at creating the kernel queue");
        
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }

    //set server listening socket as non blockin
    fcntl(listenServerFd, F_SETFL, O_NONBLOCK);

    //add event read event to kqueue
    struct kevent _kEvent;
    EV_SET(&_kEvent, listenServerFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(serverKqFd, &_kEvent, 1, NULL, 0, NULL);
    canRun = true;
    return true;
}


void server::stop()
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

void server::acceptConnection()
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
    EV_SET(&kEv, clFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
    EV_SET(&kEv, clFd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
    clientmap[clFd] = c;
    //test block
    // send(c->getClientFd(), "bruh", 4, 0);
    //test block
}

void server::disconnectClient(client *c, bool is_delete)
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

void server::read_from_client(client *c, long data_length)
{
    if (!c)
        return ; 
    char *c_buffer = new char[data_length];
    int bytesRead = recv(c->getClientFd(), c_buffer, data_length, 0);
    if (bytesRead <= 0)
        disconnectClient(c, true);
    else
    {
        c->appendToReadBuffer(c_buffer);
    }
    delete[] c_buffer;
}

void server::run()
{
    int num_events = 0;
    client *cl;
    struct kevent kEv;

    if (canRun)
    {
        //get all the triggered events
        num_events = kevent(serverKqFd, NULL, 0, _eventList, MAX_EVENTS, NULL);
        if (num_events <= 0)
            continue ;
        for (int i = 0; i < num_events; i++)
        {
            if (_eventList[i].ident == listenServerFd) //a client is waiting to connect
                acceptConnection();
            else //a client fd triggered an event
            {
                if (clientmap.find(_eventList[i].ident) == clientmap.end())
                    cl = NULL;
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
                    read_from_client(cl, _eventList[i].data);
                    //have kqueue disable tracking read events and enable write events
                    if (cl->is_reading_complete())
                    {
                        std::cout << cl->getReadBuffer() << std::endl;
                        EV_SET(&kEv, _eventList[i].ident, EVFILT_READ, EV_DISABLE, 0, 0, NULL);
                        kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                        EV_SET(&kEv, _eventList[i].ident, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);
                        kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                    }
                }
                // else if (_eventList[i].filter == EVFILT_WRITE)
                // {
                //     //have kqueue disable tracking write events and enable read events after data sending is done
                //     // EV_SET(&kEv, _eventList[i].ident, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
                //     // kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                //     // EV_SET(&kEv, _eventList[i].ident, EVFILT_READ, EV_ENABLE, 0, 0, NULL);
                //     // kevent(serverKqFd, &kEv, 1, NULL, 0, NULL);
                // }
                //TODO: a client fd can be either ready for read or write or at the end of file
                //de
            }
        }
    }
}





