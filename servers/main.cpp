#include "ServersSocketsClass.hpp"


int main(int argc, char *argv[])
{
    if (argc == 2)
        return (webserv(argv[1]));
    std::cout << "Dont you Know how to run a programm useless Bastard" << std::endl;
    return (1);
}