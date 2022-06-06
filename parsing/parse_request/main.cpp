#include "parse_request.hpp"


// int main()
// {
//     char buffer[3000] = "POST https://greenbytes.de/tech/webdav/?draft-ietf-httpbis-http2-09.html HTTP/1.1\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\n\r\n4\r\nWiki\r\n6\r\npedia \r\nE\r\nin \r\n\r\nchunks.\r\n0\r\n\r\n";

//     parse_request request;
//     std::cout << request.start_parsing(buffer, 3000) << std::endl;
// }