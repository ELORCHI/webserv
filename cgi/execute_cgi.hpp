#include "../parsing/parse_request/parse_request.hpp"


class parse_request;

class execute_cgi
{
    public:
        execute_cgi();
        ~execute_cgi();
        int start_execute_cgi(parse_request request);
        void set_environement(parse_request request);
        std::string GetPortFromHeaders();
};


