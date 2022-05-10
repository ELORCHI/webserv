/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_confile.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhebbat <yhebbat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/01 02:38:44 by yhebbat           #+#    #+#             */
/*   Updated: 2022/05/01 02:38:45 by yhebbat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFILE_HPP
#define PARSE_CONFILE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "server.hpp"
#include "location.hpp"
#include "cgi.hpp"
class cgi;
class location;
class server;
class parse_config
{
    private:
        std::vector<std::string> _lines;
        std::vector<std::string> _words;
        std::vector<server> _servers;
    public:
        parse_config():_lines(), _words(), _servers(){};
        ~parse_config(){};
        void                start_parsing();
        void                split_by_space();
        void                accolade_error();
        void                syntax_error();
        void                check_host_server_names_error();
        void                specified_words(std::string&);
        unsigned int        server_parsing(unsigned int&);
        size_t              get_lines_size() const;
        void                set_lines(std::vector<std::string> lines);
        void                read_lines();
        void                read_server();
        std::string         get_lines(int i)const;
};

#endif