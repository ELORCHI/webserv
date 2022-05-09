#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

#define PORT 8080

void download(std::string server, int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int kq = kqueue();
    struct kevent evSet;
    //EV_SET(&evSet, )
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, server.c_str(), &serv_addr.sin_addr);

    const char *message = "hello from client\n";

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    send(sock, message, strlen(message), 0);
    char rec_msg[30000] = {0};
    read(sock, rec_msg, 5000);
    printf("%s\n", rec_msg);
    // std::cout << rec_msg << std::endl;
    close(sock);
}

int main()
{
    download("127.0.0.1", 8081);
    return (0);
}