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
    _client_max_body_size(0),
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
    _upload_path = upload_path;
}

void    server::set_listen(std::string listen)
{
    if (!_listen_host.empty() || _listen_port != -1)
        throw std::runtime_error("Error: listen already set");
    std::size_t found=listen.find(':');
    if (found!=std::string::npos)
    {
        if (found == 0 && (listen.size() - found != 1))
        {
            std::string tmp;
            _listen_host = "0.0.0.0";
            tmp = listen.substr(1, listen.size());
            _listen_port = std::stoi(tmp);
        }
        else if (found == 0 && (listen.size() - found == 0))
        {
            _listen_host = "0.0.0.0" ;
            _listen_port = 80;
        }
        else
        {
            std::string tmp;
            _listen_host = listen.substr(0, found);
            tmp = listen.substr(found+1, listen.size());
            _listen_port = std::stoi(tmp);
        }
    }
    else
    {
        _listen_host = "0.0.0.0" ;
        _listen_port = 80;
    }
}

void    server::set_allowed_methods(std::string allowed_methods)
{
    _allowed_methods.push_back(allowed_methods);
}

void    server::set_index(std::string index)
{
    _index.push_back(index);
}

void    server::set_error_pages(std::string error_pages, std::string number_error)
{
    std::vector<std::string>    tmp;
    tmp.push_back(number_error);
    tmp.push_back(error_pages);
    _error_pages.push_back(tmp);
}

void    server::set_redirections(std::string redirection_from, std::string redirection_to)
{
    std::vector<std::string>    tmp;
    tmp.push_back(redirection_from);
    tmp.push_back(redirection_to);
    _redirections.push_back(tmp);
}

void    server::set_root(std::string root)
{
    _root = root;
}

void    server::set_client_max_body_size(unsigned int client_max_body_size)
{
    _client_max_body_size = client_max_body_size;
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


unsigned int    server::get_client_max_body_size() const
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
    while (i < words.size() && (words[i] == "POST" || words[i] == "GET" || words[i] == "DELETE"))
    {
        set_allowed_methods(words[i]);
        i++;
    }
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
            while (i < words.size() && words[i] != "}" && (words[i] == "POST" || words[i] == "GET" || words[i] == "DELETE"))
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