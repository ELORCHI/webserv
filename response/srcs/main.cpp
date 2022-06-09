
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>

int main(void)
{
    std::string path("test.txt");
	std::fstream in_file(path);
	std::string body = "hello\n";
	struct stat sb;

    FILE* input_file = fopen(path.c_str(), "r");
    if (input_file == nullptr) 
    {
        perror("fopen");//intrnal server error or forbiden
        return 1;
    }
	stat(path.c_str(), &sb);
    body.resize(sb.st_size);
    fread(const_cast<char*>(body.data()), sb.st_size, 1, input_file);
    fclose(input_file);
    std::cout << body << std::endl;
}