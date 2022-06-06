// #include <stdio.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <netinet/in.h>
// #include <string.h>

// #define PORT 8080
// const std::string ROOT = "./root/tata/toto";

// int main(int argc, char const *argv[])
// {
//     int server_fd, new_socket; long valread;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
    
//     // Only this line has been changed. Everything is same.
//     char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    
//     // Creating socket file descriptor
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
//     {
//         perror("In socket");
//         exit(EXIT_FAILURE);
//     }
    

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons( PORT );
    
//     memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
//     {
//         perror("In bind");
//         exit(EXIT_FAILURE);
//     }
//     if (listen(server_fd, 10) < 0)
//     {
//         perror("In listen");
//         exit(EXIT_FAILURE);
//     }
//     while(1)
//     {
//         printf("\n+++++++ Waiting for new connection ++++++++\n\n");
//         if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
//         {
//             perror("In accept");
//             exit(EXIT_FAILURE);
//         }
        
//         char buffer[30000] = {0};
//         valread = read( new_socket , buffer, 30000);
//         int fd = open("req_file", O_RDWR | O_CREATE, 0667);
//         fprintf(fd, "%s\n", buffer );
//         request.start_parsing(buffer);
//         cgi_handler();
//         // write(new_socket , hello , strlen(hello));
//         printf("------------------Hello message sent-------------------");
//         close(new_socket);
//     }
//     return 0;
// }
// #include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>

void tokenize(std::string s, std::string del = "$%")
{
    std::vector<std::string> tokens;
	int start = 0;
	int end = s.find(del);
	while (1)
    {
		tokens.push_back(s.substr(start, end - start));
		start = end + del.size();
		end = s.find(del, start);
        if (end == std::string::npos)
        {
            tokens.push_back(s.substr(start, end - start));
            break;
        }
	}
    int i = 0;
    while (i < tokens.size())
    {
        // std::cout << tokens[i].size() << std::endl;
        std::cout << tokens[i] << std::endl;
        i++;
    }
   
}
int main(int argc, char const* argv[])
{
	// Takes C++ std::string with any separator
	std::string a = "$%Hi$%do$%you$%$%do$%!$%";
	tokenize(a, "$%");

	return 0;
}
