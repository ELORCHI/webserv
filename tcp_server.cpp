#include <iostream>
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


#define PORT 8080



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
    getaddrinfo_ret = getaddrinfo("127.0.0.1", std::string("8080").c_str(), &hints, &addr);
    local_s = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    (void)bind(local_s, addr->ai_addr, addr->ai_addrlen);
    (void)listen(local_s, 5);
    return (local_s);

}

int main(void)
{
    int local_s = create_server_and_listen(PORT); 
    int kq = kqueue(); //creating an event queue
    struct kevent evSet;
    EV_SET(&evSet, local_s, EVFILT_READ, EV_ADD, 0, 0, NULL);

    //create_server_and_listen(8081);
    // const char *s = std::string("").c_str();
    // std::cout << s << std::endl;
     
    return (0);
}