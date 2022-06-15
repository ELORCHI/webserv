/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhebbat <yhebbat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/01 02:38:18 by yhebbat           #+#    #+#             */
/*   Updated: 2022/05/01 02:38:19 by yhebbat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

cgi::cgi() : _name(""), _cgi_path(""), _allow_methods(std::vector<std::string>())
{
}

cgi::~cgi()
{
}


void cgi::set_cgi_path(std::string cgi_path)
{
    if (not_predefined(cgi_path))
       this->_cgi_path = cgi_path;
    else
    {
        std::cout << "Error: cgi path not well defined" << std::endl;
        exit(0);
    } 
    
}

void cgi::set_cgi_methods(std::string methods)
{
    // std::cout << methods << std::endl;
    if (methods == "POST" || methods == "GET" || methods == "DELETE")
        this->_allow_methods.push_back(methods);
    else
    {
        std::cout << "Error: allowed methods in cgi not well defined" << std::endl;
        exit(0);
    } 
}

void    cgi::set_cgi_name(std::string name)
{
    if (not_predefined(name))
       this->_name = name;
    else
    {
        std::cout << "Error: cgi name not well defined" << std::endl;
        exit(0);
    } 
    
}

std::string cgi::get_cgi_methods(int i) const
{
    return this->_allow_methods[i];
}

std::vector<std::string> cgi::get_cgi_methods() const
{
    return this->_allow_methods;
}

std::string cgi::get_cgi_name() const
{
    return this->_name;
}

std::string cgi::get_cgi_path() const
{
    return this->_cgi_path;
}

unsigned int cgi::get_cgi_methods_size()
{
    return this->_allow_methods.size();
}

/*
    operator
*/

cgi    &cgi::operator=(cgi const &rhs)
{
    this->_name = rhs._name;
    this->_cgi_path = rhs._cgi_path;
    this->_allow_methods = rhs._allow_methods;
    return *this;
}

bool cgi::not_predefined(std::string &word) const
{
    if (word != "}" && word != "server" && word != "{" && word != "listen" 
            && word != "root" && word != "allow_methods" && word != "server_names"
            && word != "upload_path" && word != "index"
            && word != "error_page" && word != "autoindex"
            && word != "redirection" && word != "client_max_body_size"
            && word != "location" && word != "cgi" && word != "cgi_path")
            return (1);
    return (0);
}