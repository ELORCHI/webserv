#include "parse_request.hpp"


int main()
{
    char buffer[3000] = "POST https://greenbytes.de/tech/webdav/?draft-ietf-httpbis-http2-09.html HTTP/1.1\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\n\r\n26\r\nVoici les donnees du premier morceau\r\n\r\n1C\r\net voici un second morceau\r\n\r\n20\r\net voici deux derniers morceaux \r\n12\r\nsans saut de ligne\r\n0\r\n\r\n";

    parse_request request;
    std::cout << request.start_parsing(buffer, 3000) << std::endl;
}