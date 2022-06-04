/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhebbat <yhebbat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/01 02:38:21 by yhebbat           #+#    #+#             */
/*   Updated: 2022/05/01 02:38:22 by yhebbat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "server.hpp"
#include <iostream>
#include <vector>


class cgi
{
protected:
    std::string           _name;
    std::string                 _cgi_path;
    std::vector<std::string>    _allow_methods;
public:
    cgi();
    ~cgi();

    std::string                 get_cgi_path() const;
    std::string                 get_cgi_methods(int i) const;
    std::string                 get_cgi_name() const;
    std::vector<std::string>    get_cgi_methods() const;
    void                        set_cgi_path(std::string &cgi_path);
    void                        set_cgi_methods(std::string &methods);
    void                        set_cgi_name(std::string &name);
    unsigned int                get_cgi_methods_size();
    bool                        not_predefined(std::string &word) const;
    cgi                         &operator=(cgi const & rhs);
};

#endif