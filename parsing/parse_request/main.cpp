#include "parse_request.hpp"

int main()
{
    std::ifstream file("./request_file");
    std::string line;
    std::vector<std::string> lines;
    // char *buffer;
    int i = 0;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    file.close();
    size_t sz = 0;
    i =0;
    while (i < lines.size())
    {
        sz += lines[i].size();
        i++;
    }
    sz += i;
    // std::cout << sz << std::endl;
    // buffer = new char[sz];
    char buffer[sz];
    i = 0;
    int k = 0;
    while (i < lines.size())
    {
        int j = 0;
        while (j < lines[i].size())
        {
            buffer[k] = lines[i][j];
            j++;
            k++;
        }
        buffer[k] = '\n';
        k++;
        i++;
    }
    buffer[k] = '\0';
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