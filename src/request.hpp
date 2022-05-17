#pragma once
#include <string>
#include <fstream>
#include <sstream>

int skipColonNSpace(std::string astring);

class request {
    request() {}
    ~request() {}
    // static int get_content_length(std::string astring);
    public:
    static int get_content_length(std::string astring);
    static bool is_request_complete(std::string buffer);
};