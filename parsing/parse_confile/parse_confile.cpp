/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_confile.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhebbat <yhebbat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/30 18:02:58 by yhebbat           #+#    #+#             */
/*   Updated: 2022/04/30 18:02:59 by yhebbat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_confile.hpp"

//--------constructor--------

// parse_config::parse_config(): _lines()
// {
// }

// //--------destructor--------

// parse_config::~parse_config()
// {
// }

//--------methods--------

size_t parse_config::get_lines_size() const
{
    return (_lines.size());
}

void    parse_config::read_lines()
{
    for (std::vector<std::string>::iterator it = _lines.begin(); it != _lines.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
}

void    parse_config::accolade_error()
{
    size_t i = 0;
    int accolade = 0;
    while (i < _words.size())
    {
        if (_words[i] == "{")
            ++accolade;
        else if (_words[i] == "}")
            --accolade;
        ++i;
    }
    if (accolade != 0)
        throw std::runtime_error("Error: File Not Well Formated(accolade error)");
}

void       parse_config::split_by_space()
{
    for (std::vector<std::string>::iterator it = _lines.begin(); it != _lines.end(); ++it)
    {
        std::string line = *it;
        std::stringstream ss(line);
        std::string word;
        while (std::getline(ss, word, ' '))
        {
            if (word == "")
                continue;
            _words.push_back(word);
        }
    }
}

unsigned int   parse_config::server_parsing(unsigned int &i)
{
    // server *s = new server();
    server s;
    bool location_flag = false;
    bool cgi_flag = false;
    while (1)
    {
        if (i >= _words.size() || (_words[i] == "}" && !location_flag && !cgi_flag))
            break ;
        if (_words[i] == "listen")
            s.set_listen(_words[i + 1]);
        else if (_words[i] == "root")
            s.set_root(_words[i + 1]);
        else if (_words[i] == "allow_methods")
            i = s.fill_allowed_methods(_words, i);
        else if (_words[i] == "upload_path")
            s.set_upload_path(_words[i + 1]);
        else if (_words[i] == "index")
            i = s.fill_index(_words, i);
        else if (_words[i] == "error_page")
            s.set_error_pages(_words[i + 1], _words[i + 2]);
        else if (_words[i] == "autoindex")
            i = s.fill_autoindex(_words, i);
        else if (_words[i] == "redirection")
            s.set_redirections(_words[i + 1], _words[i + 2]);
        else if (_words[i] == "client_max_body_size")
            s.set_client_max_body_size(std::stoi(_words[i + 1]));
        else if (_words[i] == "location")
            i = s.fill_location(_words, i, location_flag);
        else if (_words[i] == "cgi")
           i = s.fill_cgi(_words, i, cgi_flag);
        i++;
    }
    this->_servers.push_back(s);
    return i;
}

void    parse_config::start_parsing()
{
    split_by_space();
    if (_words.size() == 0)
        throw std::runtime_error("Error: no words in the file");
    accolade_error();
    for (unsigned int i = 0; i < _words.size(); i++){
		if (_words[i] == "server")
        {
			i++;
            // std::cout << _words[i] << std::endl;
			if (_words[i] == "{")
				i = parse_config::server_parsing(i);
            i++;
		}
		else
            throw std::runtime_error("Error: every bracket must be startes by server or cgi or location");
	}
}

void    parse_config::read_data()
{
//     std::cout << this
}

void    parse_config::read_server()
{
    int i = 0;
    std::cout << "listen_host: " << this->_servers[i].get_listen_host() << std::endl;
    std::cout << "listen_port: " <<this->_servers[i].get_listen_port() << std::endl;
    std::cout << "root: " << this->_servers[i].get_root() << std::endl;
    unsigned int j = 0;
    std::cout << "allowed_methods: ";
    while (j < this->_servers[i].get_allowed_methods_size())
    {
        std::cout << this->_servers[i].get_allowed_methods(j) << " ";
        j++;
    }
    std::cout << std::endl;
    std::cout << "upload_path: " << this->_servers[i].get_upload_path() << std::endl;
    j = 0;
    std::cout << "index: ";
    while (j < this->_servers[i].get_index_size())
    {
        std::cout << this->_servers[i].get_index(j) << " ";
        j++;
    }
    std::cout << std::endl;
    if (this->_servers[i].get_autoindex())
        std::cout << "auto index: on" << std::endl;
    else
        std::cout << "auto index: on" << std::endl;
    j = 0;
    while (j < _servers[i].get_error_pages_size())
    {
        std::cout << "error_pages: ";
        std::vector<std::string> tmp;
        tmp = _servers[i].get_error_pages(j);
        std::cout << tmp[0];
        std::cout << "   ";
        std::cout << tmp[1] << std::endl;
        j++;
    }
    j = 0;
    while (j < _servers[i].get_redirections_size())
    {
        std::cout << "redirections: ";
        std::vector<std::string> tmp;
        tmp = _servers[i].get_redirections(j);
        std::cout << tmp[0];
        std::cout << "   ";
        std::cout << tmp[1] << std::endl;
        j++;
    }
    std::cout << "client_max_body_size: " << this->_servers[i].get_client_max_body_size() << std::endl;

    location t = this->_servers[0].get_location(0);
    std::cout << t.get_root() << std::endl;
}
//--------getters-------------

std::string parse_config::get_lines(int i) const
{
    return (_lines[i]);
}
//--------setters-------------

void   parse_config::set_lines(std::vector<std::string> lines)
{
    _lines = lines;
}

//--------operators--------