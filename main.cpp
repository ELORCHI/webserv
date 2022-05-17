#include "src/server.hpp"
#include <string>
#include <iostream>
#include "src/request.hpp"
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
#include <streambuf>


int main(int argc, char *argv[])
{

    server s(4201);
    s.start();
    s.run();
    // std::stringstream ss;
    // std::string s;
    // std::ifstream file;
    // file.open("test.txt", std::fstream::in); 
    // file.seekg(0, std::ios::end);
    // s.reserve(file.tellg());
    // file.seekg(0, std::ios::beg);
    // s.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    //std::cout << s << std::endl;
    // std::cout << request::is_request_complete(s);
    // std::cout << s.substr(s.find("Content-Length")) << std::endl;
   // int cl;
   // cl << skipColonNSpace("Content-Length: 260632");
    // std::cout << skipColonNSpace("Content-Length: 260632");;
    // ss << file;

    
    // std::cout << ss << std::endl;
    return (0);
}