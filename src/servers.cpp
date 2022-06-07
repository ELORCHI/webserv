#include "servers.hpp"




httpServers::httpServers(int argc, char **argv)
{
    parse_config conf;
    std::vector<server> parsed_servers_data;
    std::map<int, socket_data *> shared_sockets;
    std::set<int> bindedSharedPorts;
    if(!httpServer::parsing_conf(argc, argv, &conf))
		return ;

	parsed_servers_data = conf.get_server_vect();
    std::set<int> sharedPorts = httpServer::getRepeatedPorts(parsed_servers_data);
    
    for (int i = 0; i < parsed_servers_data.size(); i++)
    {
        int port = parsed_servers_data[i].get_listen_port();
        if (std::find(sharedPorts.begin(), sharedPorts.end(), port) != sharedPorts.end()) {
            if (bindedSharedPorts.find(port) == bindedSharedPorts.end()) {
                shared_sockets[port] =  httpServer::create_listening_socket(port, parsed_servers_data[i].get_listen_host());
                bindedSharedPorts.insert(port);
            }
        }
    }
    if ((this->KqueueFd = kqueue()) == -1)
        throw MyException("failure at creating the kernel queue");

    for (int i = 0; i < parsed_servers_data.size(); i++)
    {
        int port = parsed_servers_data[i].get_listen_port();
        if (std::find(sharedPorts.begin(), sharedPorts.end(), port) != sharedPorts.end())
        {
            socket_data *sd = shared_sockets[port];
            _servers.push_back(httpServer(parsed_servers_data[i], true, sd, KqueueFd));
            // httpServer::httpServer kk(parsed_servers_data[i], true, sd);
        }
        else
            _servers.push_back(httpServer(parsed_servers_data[i], false, NULL, KqueueFd));
    }
    //std::vector<int> shared_ports = httpServer::getRepeatedPorts(parsed_servers_data);
    // std::cout << parsed_servers_data.size() << std::endl; 

}

void httpServers::httpServers_repl()
{
    int num_events = 0;
    while (1)
    {
        for (int i = 0; i < _servers.size(); i++)
		{
            num_events = kevent(KqueueFd, NULL, 0, _eventList, MAX_EVENTS, NULL);
			// std::cerr << "WEWEWEWEWEWEWEWEWEWEWEWE" << std::endl;
            _servers[i].run(num_events, _eventList);
			// std::cerr << "WAWAWAWAWAWAWAWAWAWAWAWA" << std::endl;
		}
	}
}