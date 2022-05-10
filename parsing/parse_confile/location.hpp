/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhebbat <yhebbat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/01 02:38:30 by yhebbat           #+#    #+#             */
/*   Updated: 2022/05/01 02:38:31 by yhebbat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "server.hpp"
#include <iostream>
#include <vector>

class cgi;
class location
{
private:
    const std::string           _name;
    std::string                 _locations_path;
    std::vector<std::string>    _allow_methods;
    std::string                 _root;
    std::string                 _client_max_body_size;
    std::vector<std::string>    _index;
    std::vector<cgi>                        _cgi;
    bool                         _autoindex;
public:
    location();
    ~location();

    std::string                 &get_locations_path();
    std::vector<std::string>    &get_methods();
    std::string                 &get_root();
    bool                        &get_autoindex();
    std::string                 &get_client_max_body_size();
    std::string                 get_index(unsigned int i) const;
    unsigned int                get_index_size() const;
    std::string                 get_methods(unsigned int i) const;
    unsigned int                get_methods_size() const;
    void                        set_locations_path(std::string &locations_path);
    void                        set_methods(std::string &methods);
    void                        set_root(std::string &root);
    void                        set_autoindex(bool autoindex);
    void                        set_client_max_body_size(std::string &client_max_body_size);
    void                        set_index(std::string &index);
    void                         set_cgi(cgi cgi);
    unsigned int                 get_cgi_size() const;
    cgi                          get_cgi(int i) const;
    unsigned int                 fill_allowed_methods(std::vector<std::string>, unsigned int);
    unsigned int                 fill_index(std::vector<std::string>, unsigned int);
    unsigned int                 fill_autoindex(std::vector<std::string>, unsigned int);
    unsigned int                 fill_cgi(std::vector<std::string>, unsigned int, bool&);
};

#endif
