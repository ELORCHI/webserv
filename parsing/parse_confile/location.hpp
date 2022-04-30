/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhebbat <yhebbat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/30 18:02:45 by yhebbat           #+#    #+#             */
/*   Updated: 2022/04/30 18:02:46 by yhebbat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "server.hpp"
#include <iostream>
#include <vector>

class location
{
private:
    const std::string           _name;
    std::string                 _locations_path;
    std::vector<std::string>    _allow_methods;
    std::string                 _root;
    std::string                 _client_max_body_size;
    std::vector<std::string>    _index;
    bool                         _autoindex;
public:
    location();
    ~location();

    std::string                 &get_locations_path();
    std::vector<std::string>    &get_methods();
    std::string                 &get_root();
    bool                        &get_autoindex();
    std::string                 &get_client_max_body_size();
    std::vector<std::string>    &get_index();
    void                        set_locations_path(std::string &locations_path);
    void                        set_methods(std::string &methods);
    void                        set_root(std::string &root);
    void                        set_autoindex(bool autoindex);
    void                        set_client_max_body_size(std::string &client_max_body_size);
    void                        set_index(std::string &index);
    unsigned int                 fill_allowed_methods(std::vector<std::string>, unsigned int);
    unsigned int                 fill_index(std::vector<std::string>, unsigned int);
    unsigned int                 fill_autoindex(std::vector<std::string>, unsigned int);
};

#endif
