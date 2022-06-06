/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhebbat <yhebbat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/01 02:38:56 by yhebbat           #+#    #+#             */
/*   Updated: 2022/05/01 02:38:57 by yhebbat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>  
#include <string>  
#include <vector>  
#include <arpa/inet.h>
#include <sstream>  
using namespace std;  


// int main()
// {
//     std::vector<std::vector<std::string>> v;
//     std::vector<std::string> v2;
//     v2.push_back("asa");
//     v2.push_back("ssss");
//     v.push_back(v2);
//     v2.push_back("1");
//     v2.push_back("2");
//     v2.push_back("3");
//     v.push_back(v2);
//     int i = 0;
//     int j = 0;
//     while (i < v.size())
//     {
//         while (j < v[i].size())
//         {
//             std::cout << v[i][j] << " ";
//             j++;
//         }
//         std::cout << std::endl;
//         i++;
//     }
//     return (0);
// }

// int main()  
// {  
//     string S, T;  // declare string variables  
   
//     getline(cin, S); // use getline() function to read a line of string and store into S variable.  
   
//     stringstream X(S); // X is an object of stringstream that references the S string  
   
//     // use while loop to check the getline() function condition  
//     while (getline(X, T, ' ')) {  
//         /* X represents to read the string from stringstream, T use for store the token string and, 
//          ' ' whitespace represents to split the string where whitespace is found. */  
          
//         cout << T << endl; // print split string  
//     }
//     return 0;  
// }  

// int main(){
//     unsigned long host = inet_addr("192.1.0.2"); 

//     cout << host << endl;   
//     cout << ntohl(host) << endl;
// }


// std::string names[] = {"robotomy request","shrubbery creation","presidential pardon"};
// 	Form* (Intern::*fct[])(std::string) = 
// 	{
// 		&Intern::RobotomyRequest, &Intern::ShrubberyCreation, &Intern::PresidentialPardon, &Intern::nothing
// 	};
    
// size_t		Config::parseServer(configFile con, unsigned int &index){
// 	serverConfig *server = new serverConfig();
// 	bool isLocation = 0;
// 	Ptr values[7] = {&serverConfig::serverName, &serverConfig::root,
// 	&serverConfig::listen, &serverConfig::parseLocation, &serverConfig::allowMethods,
// 	&serverConfig::index, &serverConfig::errorPages};
// 	while (true)
// 	{
// 		index++;
// 		for (size_t i = 0; i < 7; i++)
// 		{
// 			if (i == 3)
// 				isLocation = 1;
// 			if (index >= con.size())
// 				break; 
// 			if (con[index] == keys[i])
// 				index = (server->*values[i])(*server, con, index);
// 		}
// 		if (index >= con.size() || (con[index] == "}" && !isLocation) || !con[index].compare("server"))
// 			break;
// 	}
// 	// ? check if two servers have the same server_name and the same port
// 	this->servers.push_back(server);
// 	index--;
// 	return index; //! return index to the last colla 
// }