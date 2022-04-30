/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhebbat <yhebbat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/30 18:02:29 by yhebbat           #+#    #+#             */
/*   Updated: 2022/04/30 18:02:30 by yhebbat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "server.hpp"
#include <iostream>
#include <vector>


class cgi
{
private:
    std::string           _name;
    std::string                 _cgi_path;
    std::vector<std::string>    _allow_methods;
public:
    cgi();
    ~cgi();

    std::string                 &get_cgi_path();
    std::string                 &get_cgi_methods(int i);
    std::string                 &get_cgi_name();
    void                        set_cgi_path(std::string &cgi_path);
    void                        set_cgi_methods(std::string &methods);
    void                        set_cgi_name(std::string &name);
};

#endif