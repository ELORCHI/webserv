#include "parse_request.hpp"

int main()
{
    char buffer[221] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nTransfer-Encoding: unchunked\r\n\r\n<html><body>\r\n<h1>Hello, World!</h1>\r\n</body>\r\n</html>\r\n";
    
    // 26\r\n
    // Voici les données du premier morceau\r\n
    // \r\n
    // 1C\r\n
    // et voici un second morceau\r\n
    // \r\n
    // 20\r\n
    // et voici deux derniers morceaux \r\n
    // 12\r\n
    // sans saut de ligne\r\n
    // 0\r\n
    // \r\n
    
    
    // std::ifstream file("./request_file");
    // std::string line;
    // std::vector<std::string> lines;
    // // char *buffer;
    // int i = 0;
    // while (std::getline(file, line))
    // {
    //     lines.push_back(line);
    // }
    // file.close();
    // size_t sz = 0;
    // i =0;
    // while (i < lines.size())
    // {
    //     sz += lines[i].size();
    //     i++;
    // }
// sz += i;
    // char buffer[sz];
    // i = 0;
    // int k = 0;
    // while (i < lines.size())
    // {
    //     int j = 0;
    //     while (j < lines[i].size())
    //     {
    //         buffer[k] = lines[i][j];
    //         j++;
    //         k++;
    //     }
    //     buffer[k] = '\n';
    //     k++;
    //     i++;
    // }
    // buffer[k] = '\0';
    // buffer = new char[lines.size()];
    // while (i < lines.size())
    // {
    //     buffer = new char[lines[i].size()];
    //     std::strcpy(buffer, lines[i].c_str());
    //     ++i;
    // }
    // buffer[i] = NULL;
    parse_request request;
    request.start_parsing(buffer);
}