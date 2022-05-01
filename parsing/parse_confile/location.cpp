/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhebbat <yhebbat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/01 02:38:24 by yhebbat           #+#    #+#             */
/*   Updated: 2022/05/01 02:38:25 by yhebbat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

location::location() : _locations_path(""),
                        _allow_methods(std::vector<std::string>()),
                        _root(""),
                        _client_max_body_size(""),  
                        _index(std::vector<std::string>()),
                        _autoindex(false)
                        {}
location::~location() {}

/*
* GETTERS
*/
std::string                 &location::get_locations_path() { return this->_locations_path; }
std::vector<std::string>    &location::get_methods() { return this->_allow_methods; }
std::string                 &location::get_root() { return this->_root; }
bool                        &location::get_autoindex() { return this->_autoindex; }
std::string                 &location::get_client_max_body_size() { return this->_client_max_body_size; }

/*
* SETTERS
*/
void location::set_locations_path(std::string &locations_path) { this->_locations_path = locations_path; }
void location::set_methods(std::string &methods){ this->_allow_methods.push_back(methods);}
void location::set_root(std::string &root) { this->_root = root; }
void location::set_autoindex(bool autoindex) { this->_autoindex = autoindex; }
void location::set_client_max_body_size(std::string &client_max_body_size) { this->_client_max_body_size = client_max_body_size; }
void location::set_index(std::string &index) { this->_index.push_back(index); }

/*
* methods
*/


unsigned int location::fill_allowed_methods(std::vector<std::string> words, unsigned int i)
{
    i++;
    while (i < words.size() && (words[i] == "POST" || words[i] == "GET" || words[i] == "DELETE"))
    {
        set_methods(words[i]);
        i++;
    }
    i--;
    return i;
}

unsigned int location::fill_index(std::vector<std::string> words, unsigned int i)
{
    i++;
    while (i < words.size() && words[i] != "}" && words[i] != "server" && words[i] != "{" && words[i] != "listen" 
            && words[i] != "root" && words[i] != "allow_methods"
            && words[i] != "upload_path" && words[i] != "index"
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

unsigned int location::fill_autoindex(std::vector<std::string> words, unsigned int i)
{
    if (words[i + 1] == "on")
        set_autoindex(true);
    else
        set_autoindex(false);
    return i;
}

unsigned int location::get_index_size() const
{
    return (_index.size());
}


std::string                 location::get_index(unsigned int i) const
{
    return  (this->_index[i]);
}

unsigned int location::get_methods_size() const
{
    return (_allow_methods.size());
}


std::string                 location::get_methods(unsigned int i) const
{
    return  (this->_allow_methods[i]);
}
unsigned int location::fill_cgi(std::vector<std::string> words, unsigned int i, bool &cgi_flag)
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

void    location::set_cgi(cgi cgi)
{
    _cgi.push_back(cgi);
}

unsigned int location::get_cgi_size() const
{
    return _cgi.size();
}

cgi     location::get_cgi(int i) const
{
    return _cgi[i];
}
