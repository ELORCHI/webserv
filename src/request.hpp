#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "../parsing/parse_request/parse_request.hpp"


int skipColonNSpace(std::string astring);

class request {
    request(std::string request_text)
    {

    }
    ~request() {}
    // static int get_content_length(std::string astring);
    public:
        static int get_content_length(std::string astring);
        static bool is_request_complete(std::string buffer);
        static bool is_chunked_request_complete(std::string buffer);
};