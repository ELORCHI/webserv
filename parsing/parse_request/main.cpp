#include "parse_request.hpp"




// int main()
// {
//     std::string ff = "hello";
// 	std::string newf = "word";
	
// 	int new_fd = open(newf.c_str(), O_RDWR | O_CREAT | O_APPEND, 0644);
// 	int old_fd = open(ff.c_str(), O_RDONLY, 0777);
// 	char buffer[2000] = {0};
// 	int ret = 1;

// 	if (new_fd == -1 || old_fd == -1)
// 		return (500);
// 	lseek(old_fd, 0, SEEK_SET);
// 	int i = 0;
//     while (ret > 0)
// 	{
//         std::cout << i << std::endl;
// 		memset(buffer, 0, 2000);
// 		ret = read(old_fd, buffer, 2000 - 1);
// 		if (ret >= 0)
//         {
// 			write(new_fd, buffer, ret);
//             std::cout << ret << std::endl;

//         }
//         else
//             return 500;
// 	}
// 	close(new_fd);
// 	close(old_fd);
//     unlink(ff.c_str());
// 	remove(ff.c_str());
// 	return (1);
// }