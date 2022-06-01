/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhebbat <yhebbat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/01 02:38:48 by yhebbat           #+#    #+#             */
/*   Updated: 2022/05/01 02:38:49 by yhebbat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server.hpp"
#include <iostream>  
#include <string>  
#include <vector>  
#include <sstream>  
#include "./location.hpp"
#include "./cgi.hpp"

//-------------constructor--------------
server::server():
    _name(),
    _listen_port(-1),
    _listen_host(),
    _allowed_methods(),
    _index(),
    _error_pages(),
    _redirections(),
    _root(""),
    _client_max_body_size(-1),
    _autoindex(false)
{
}
    // _location(),
    // _cgi()
std::string server_names[] = {"listen","root","allow_methods", "upload_path", "index", "error_page", "autoindex", "redirection"};

//-------------destructor--------------
server::~server()
{
}

//-------------seters------------------

void    server::set_name(std::string name)
{
    _name.push_back(name);
}

void    server::set_upload_path(std::string upload_path)
{
    if (not_predefined(upload_path))
        _upload_path = upload_path;
    else
        throw std::runtime_error("Error: upload_path is empty");
}

void    server::set_to_default()
{
    if (_allowed_methods.empty() || _root.empty())
        throw std::runtime_error("Error: server need more info!!");
    if (_listen_host.empty() || _listen_port == -1)
    {
        _listen_host = "0.0.0.0" ;
        _listen_port = 80;
    }
}

void    server::check_host(std::string listen_host)
{
    if (listen_host != "localhost")
    {
        int i = 0;
        int t = 0;
        while (listen_host[i])
        {
            if (listen_host[i] == '.')
                t++;
            i++;
        }
        if (t != 3)
            throw std::runtime_error("Error: host not valid");
        i = 0;
        char * cstr = new char [listen_host.length()+1];
        std::strcpy (cstr, listen_host.c_str());
        char *p = std::strtok (cstr,".");
        while (p!=0)
        {
            if (!is_number(p))
            {
                delete[] cstr;
                throw std::runtime_error("Error: host not valid");
            }
            int tmp = std::atoi(p);
            if (tmp < 0 || tmp > 255)
            {
                delete[] cstr;
                throw std::runtime_error("Error: host not valid");
            }
            i++;
            p = std::strtok (NULL,".");
        }
        delete[] cstr;
        if (i != 4)
            throw std::runtime_error("Error: host not valid");
    }
}
void    server::set_listen(std::string listen)
{
    if (!_listen_host.empty() || _listen_port != -1)
        throw std::runtime_error("Error: listen already set");
    std::size_t found=listen.find(':');
    if (found != std::string::npos)
    {
        if (found == 0 && (listen.size() - found != 1))
        {
            std::string tmp;
            _listen_host = "0.0.0.0";
            tmp = listen.substr(1, listen.size());
            if (is_number(tmp))
                _listen_port = std::stoi(tmp);
            else
                throw std::runtime_error("Error: port should be a number");
        }
        else if (found == 0 && (listen.size() - found == 1))
        {
            _listen_host = "0.0.0.0" ;
            _listen_port = 80;
        }
        else
        {
            check_host(listen.substr(0, found));
            _listen_host = listen.substr(0, found);
            std::string tmp;
            _listen_host = listen.substr(0, found);
            tmp = listen.substr(found+1, listen.size());
            if (is_number(tmp))
                _listen_port = std::stoi(tmp);
            else
                throw std::runtime_error("Error: port should be a number");
        }
    }
    else
    {
        check_host(listen);
        _listen_host = listen;
        _listen_port = 80;
    }
}

void    server::set_allowed_methods(std::string allowed_methods)
{
    if (allowed_methods == "POST" || allowed_methods == "GET" || allowed_methods == "DELETE")
        _allowed_methods.push_back(allowed_methods);
    else
        throw std::runtime_error("Error: allowed methods not well defined");
}

void    server::set_index(std::string index)
{
    _index.push_back(index);
}

void    server::set_error_pages(std::string error_pages, std::string number_error)
{
    if (!not_predefined(error_pages) || !not_predefined(number_error))
        throw std::runtime_error("Error: error_pages or number_of_error not well defined");
    else
    {
        std::vector<std::string>    tmp;
        tmp.push_back(number_error);
        tmp.push_back(error_pages);
        _error_pages.push_back(tmp);
    }
}

void    server::set_redirections(std::string redirection_from, std::string redirection_to)
{
    if (!not_predefined(redirection_from) || !not_predefined(redirection_to))
        throw std::runtime_error("Error: redirections not well defined");
    else
    {
        std::vector<std::string>    tmp;
        tmp.push_back(redirection_from);
        tmp.push_back(redirection_to);
        _redirections.push_back(tmp);
    }
}

void    server::set_root(std::string root)
{
    if (not_predefined(root))
        _root = root;
    else
        throw std::runtime_error("Error: root not well defined");
}

bool server::is_number(const std::string& str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		char current = str[i];
		if (current >= '0' && current <= '9')
			continue;
		return false;
	}
	return true;
}

void    server::set_client_max_body_size(std::string client_max_body_size)
{
    if (not_predefined(client_max_body_size) && is_number(client_max_body_size))
        _client_max_body_size =std::stoi(client_max_body_size);
    else
        throw std::runtime_error("Error: client max body size should be number");
}

void    server::set_autoindex(bool autoindex)
{
    _autoindex = autoindex;
}

void   server::set_location(location location)
{
    _location.push_back(location);
}

void    server::set_cgi(cgi cgi)
{
    _cgi.push_back(cgi);
}

//-------------geters------------------

std::string    server::get_name(int i) const
{
    return _name[i];
}

std::string    server::get_upload_path() const
{
    return _upload_path;
}

std::string   server::get_listen_host() const
{
    return _listen_host;
}

int   server::get_listen_port() const
{
    return _listen_port;
}

std::string  server::get_allowed_methods(int i) const
{
    return _allowed_methods[i];
}

std::vector<std::string> server::get_error_pages(int i) const
{
    return _error_pages[i];
}

unsigned int server::get_error_pages_size() const
{
    return _error_pages.size();
}

unsigned int server::get_redirections_size() const
{
    return _redirections.size();
}

std::vector<std::string> server::get_redirections(int i) const
{
    return _redirections[i];
}

std::string server::get_root() const
{
    return _root;
}

bool   server::get_autoindex() const
{
    return _autoindex;
}


long long int    server::get_client_max_body_size() const
{
    return _client_max_body_size;
}

location   server::get_location(int i) const
{
    return _location[i];
}

cgi     server::get_cgi(int i) const
{
    return _cgi[i];
}

unsigned int    server::get_allowed_methods_size() const
{
    return _allowed_methods.size();
}

//----------methods

unsigned int server::fill_allowed_methods(std::vector<std::string> words, unsigned int i)
{
    i++;
    while (i < words.size() && not_predefined(words[i]))
    {
        set_allowed_methods(words[i]);
        i++;
    }
    if (get_allowed_methods_size() == 0)
            throw std::runtime_error("Error: server allow_method is empty");
    i--;
    return i;
}

unsigned int server::fill_name(std::vector<std::string> words, unsigned int i)
{
    i++;
    while (i < words.size() && words[i] != "}" && words[i] != "server" && words[i] != "{" && words[i] != "listen" 
            && words[i] != "root" && words[i] != "allow_methods" && words[i] != "server_names"
            && words[i] != "upload_path" && words[i] != "index"
            && words[i] != "error_page" && words[i] != "autoindex"
            && words[i] != "redirection" && words[i] != "client_max_body_size"
            && words[i] != "location" && words[i] != "cgi")
    {
        set_name(words[i]);
        i++;
    }
    if (get_name_size() == 0)
            throw std::runtime_error("Error: server name is empty");
    i--;
    return i;
}

unsigned int server::fill_index(std::vector<std::string> words, unsigned int i)
{
    i++;
    while (i < words.size() && words[i] != "}" && words[i] != "server" && words[i] != "{" && words[i] != "listen" 
            && words[i] != "root" && words[i] != "allow_methods"
            && words[i] != "upload_path" && words[i] != "index" && words[i] != "server_names"
            && words[i] != "error_page" && words[i] != "autoindex"
            && words[i] != "redirection" && words[i] != "client_max_body_size"
            && words[i] != "location" && words[i] != "cgi")
    {
        set_index(words[i]);
        i++;
    }
    if (get_index_size() == 0)
            throw std::runtime_error("Error: server index is empty");
    i--;
    return i;
}

unsigned int server::fill_autoindex(std::vector<std::string> words, unsigned int i)
{
    if (words[i + 1] == "on")
        set_autoindex(true);
    else
        set_autoindex(false);
    return i;
}

unsigned int server::fill_location(std::vector<std::string> words, unsigned int i, bool &location_flag)
{
    location_flag = true;
    bool cgi_flag = false;
    location l;
    l.set_locations_path(words[i + 1]);
    while (1)
    {
        if (i >= words.size() || (words[i] == "}" && location_flag))
            break ;
        if (words[i] == "root")
            l.set_root(words[i + 1]);
        else if (words[i] == "allow_methods")
            i = l.fill_allowed_methods(words, i);
        else if (words[i] == "index")
            i = l.fill_index(words, i);
        else if (words[i] == "autoindex")
            i = l.fill_autoindex(words, i);
        else if (words[i] == "cgi")
            i = l.fill_cgi(words, i, cgi_flag);
        i++;
    }
    location_flag = false;
    set_location(l);
    if (l.get_methods().empty() || l.get_root().empty())
        throw std::runtime_error("Error: location need more info!!");
    return i;
}

unsigned int server::get_index_size() const
{
    return (_index.size());
}

unsigned int server::get_location_size() const
{
    return _location.size();
}

unsigned int server::get_cgi_size() const
{
    return _cgi.size();
}

unsigned int server::get_name_size() const
{
    return _name.size();
}

std::string                  server::get_index(int i) const
{
    return  (_index[i]);
}


unsigned int server::fill_cgi(std::vector<std::string> words, unsigned int i, bool &cgi_flag)
{
    cgi_flag = true;
    cgi c;
    c.set_cgi_name(words[i + 1]);
    while (1)
    {
        if (i >= words.size() || (words[i] == "}" && cgi_flag))
            break ;
        if (words[i] == "cgi_path")
            c.set_cgi_path(words[i + 1]);
        else if (words[i] == "allow_methods")
        {
            i++;
            while (i < words.size() && not_predefined(words[i]))
            {
                c.set_cgi_methods(words[i]);
                i++;
            }
            i--;
        }
        i++;
    }
    cgi_flag = false;
    set_cgi(c);
    return i;
}

/*
    operator
*/

server    &server::operator=(server const &rhs)
{
    if (this != &rhs)
    {
        _name = rhs._name;
        _index = rhs._index;
        _autoindex = rhs._autoindex;
        _client_max_body_size = rhs._client_max_body_size;
        _allowed_methods = rhs._allowed_methods;
        _location = rhs._location;
        _cgi = rhs._cgi;
        _redirections = rhs._redirections;
        _root = rhs._root;
    }
    return *this;
}

bool server::not_predefined(std::string &word) const
{
    if (word != "}" && word != "server" && word != "{" && word != "listen" 
            && word != "root" && word != "allow_methods" && word != "server_names"
            && word != "upload_path" && word != "index"
            && word != "error_page" && word != "autoindex"
            && word != "redirection" && word != "client_max_body_size"
            && word != "location" && word != "cgi")
            return (1);
    return (0);
}

std::vector<location>    &server::get_location()
{
    return _location;
}

std::vector<cgi>    &server::get_cgi()
{
    return _cgi;
}