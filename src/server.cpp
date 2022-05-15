#include "server.hpp"

server::server(int port)
{
    listenServerFd = INVALID_SOCKET;
    listenServerPort = port; 
    canRun = false;
    serverKqFd = -1;
    memset(&listeningServAddr, 0, sizeof listeningServAddr);
    listeningServAddr.sin_family = AF_INET; //IP v4 family
    listeningServAddr.sin_port = htons(listenServerPort); //The htons function takes a 16-bit number in host byte order and returns a 16-bit number in network byte order used in TCP/IP networks
    listeningServAddr.sin_addr.s_addr = INADDR_ANY; //accept all connections aka set address to 0.0.0.0
}


bool server::start() {
    canRun = false;
    listenServerFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    try {
        if (listenServerFd == INVALID_SOCKET)
            throw MyException("failed at creating the server socket!");
        int bind_r = bind(listenServerFd, (struct sockaddr*)&listeningServAddr, sizeof(listeningServAddr));
        if (bind_r != 0) 
            throw MyException("binding address to socket failed!");
        if (listen(listenServerPort, 1000) != 0) {
            throw MyException("binding address to server socket failed!");
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


void server::run()
{
    int num_events = 0;
    while (canRun)
    {
        //get all the triggered events
        num_events = kevent(serverKqFd, NULL, 0, _eventList, MAX_EVENTS, NULL);
        if (num_events <= 0)
            continue ;
        for (int i = 0; i < num_events; i++)
        {
            if (_eventList[i].ident == listenServerFd) //a client is waiting to connect
            {
                //accept connection
            }
            else //a client fd triggered an event
            {
                
            }
        }
    }
}