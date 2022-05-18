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

void cgi::set_cgi_path(std::string &cgi_path)
{
    this->_cgi_path = cgi_path;
}

void cgi::set_cgi_methods(std::string &methods)
{
    this->_allow_methods.push_back(methods);
}

void    cgi::set_cgi_name(std::string &name)
{
    this->_name = name;
}

std::string &cgi::get_cgi_methods(int i)
{
    return this->_allow_methods[i];
}

std::string &cgi::get_cgi_name()
{
    return this->_name;
}

std::string &cgi::get_cgi_path()
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