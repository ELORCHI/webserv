#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>



void server(int port)
{
    const char *srv_msg = "hello from server\n";
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    int addrlen = sizeof(serv_addr);

    bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(sock, 10);

    while (1)
    {
        int new_socket = accept(sock, (struct sockaddr *)&serv_addr, (socklen_t *)&addrlen);
        char buffer[30000] = {0};
        read (new_socket, buffer, 30000);
        std::cout << buffer << std::endl;
        write(new_socket, srv_msg, strlen(srv_msg));
    }

}

int main(void)
{
    server(8081);
    return (0);
}