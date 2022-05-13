#include <iostream>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>



#define NUM_CLIENTS 10
#define PORT 1236 
#define MAX_EVENTS 32
#define MAX_MSG_SIZE 256



struct client_data
{
    int fd;
} cients[NUM_CLIENTS];

// void create_server_and_listen(int port)
// {
//     const char *srv_msg = "hello from server\n";
//     int sock = socket(AF_INET, SOCK_STREAM, 0);
//     struct sockaddr_in serv_addr;
//     memset(&serv_addr, 0, sizeof(serv_addr));
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(port);
//     serv_addr.sin_addr.s_addr = INADDR_ANY;
//     int addrlen = sizeof(serv_addr);

//     bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
//     listen(sock, 10);

//     while (1)
//     {
//         int new_socket = accept(sock, (struct sockaddr *)&serv_addr, (socklen_t *)&addrlen);
//         char buffer[30000] = {0};
//         read (new_socket, buffer, 30000);
//         std::cout << buffer << std::endl;
//         write(new_socket, srv_msg, strlen(srv_msg));
//     }

// }

int create_server_and_listen(int port)
{
    struct addrinfo *addr;
    struct addrinfo hints;
    int getaddrinfo_ret;
    int local_s;
    memset(&hints, 0, sizeof hints);
    hints.ai_flags = AI_PASSIVE; // Setting the AI_PASSIVE flag indicates the caller intends to use the returned socket address structure in a call to the bind function.
    hints.ai_family = PF_UNSPEC; //A value of AF_UNSPEC for ai_family indicates the caller will accept only the AF_INET (ip v 4) and AF_INET6 address families
    hints.ai_socktype = SOCK_STREAM;
    std::stringstream ss;
    ss << PORT;
    getaddrinfo_ret = getaddrinfo("127.0.0.1", ss.str().c_str(), &hints, &addr);
    local_s = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    (void)bind(local_s, addr->ai_addr, addr->ai_addrlen);
    (void)listen(local_s, 5);
    return (local_s);
}

void test_send_data_to_client(int s)
{
    char msg[80];
    sprintf(msg, "this is a message from the server");
    send(s, msg, strlen(msg), 0);
}

void recv_msg(int s) {
    char buf[MAX_MSG_SIZE];
    std::string str; 
    int bytes_read;
    while ((bytes_read = recv(s, buf, sizeof(buf) - 1, 0)) > 0)
    {
        str = str + std::string(buf);
    }

    // buf[bytes_read] = 0;
    //printf("client #%d: %s", get_conn(s), buf);
    fflush(stdout);
    std::cout << str;
}

void send_welcome_msg(int s) {
    char msg[80];
    sprintf(msg, "dude comeon don't do this");
    send(s, msg, strlen(msg), 0);
}

void run_event_loop(int kq, int local_s)
{
    struct kevent evSet;
    struct kevent evList[MAX_EVENTS];
    struct sockaddr_storage addr;
    socklen_t socklen = sizeof(addr);

    while (1)
    {
        int num_events = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL); //get list of triggered events
        for (int i = 0; i < num_events; i++)
        {
            if (evList[i].ident == local_s) //if triggered event identifier is the same as the our server soccket fd then a client is trying to connect
            {
                int fd = accept(evList[i].ident, (struct sockaddr *)&addr, &socklen); //establishing connection with a client
                EV_SET(&evSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL); // set an event to get triggered  when client fd is ready for read
                kevent(kq, &evSet, 1, NULL, 0, NULL); // add the event to the kqueue
                EV_SET(&evSet, fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
                kevent(kq, &evSet, 1, NULL, 0, NULL);
                //send data to client
                 test_send_data_to_client(fd);
            }
            else if (evList[i].flags & EV_EOF) {
            /*
            ** When a client disconnects, we receive an event where the EOF flag
            ** is set on the socket. We simply free up that connection in the pool
            ** and remove the event from kqueue (via EV_DELETE).
            */
                int fd = evList[i].ident;
                EV_SET(&evSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                kevent(kq, &evSet, 1, NULL, 0, NULL);
                // EV_SET(&evSet, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                // kevent(kq, &evSet, 1, NULL, 0, NULL);
            }
            else if (evList[i].filter & EVFILT_READ) {
                //read data from evList[i].ident
                recv_msg(evList[i].ident);
                // test_send_data_to_client(evList[i].ident);
                // send_welcome_msg(evList[i].ident); 
            } 
            else if (evList[i].filter & EVFILT_WRITE) {
                test_send_data_to_client(evList[i].ident);
            }
        }
    }
}


int main(void)
{
    int local_s = create_server_and_listen(PORT); 
    int kq = kqueue(); //creating an event queue
    struct kevent evSet;
    EV_SET(&evSet, local_s, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL); //setting the event for being triggered whenever there's data to read
    kevent(kq, &evSet, 1, NULL, 0, NULL); // adding the event to the kqueue
    run_event_loop(kq, local_s);

    return (0);
}