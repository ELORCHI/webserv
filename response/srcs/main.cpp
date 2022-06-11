#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// POSIX dependencies
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../includes/response.hpp"

// int rmtree(const char path[])
// {
//     size_t path_len;
//     char *full_path;
//     DIR *dir;
//     struct stat stat_path, stat_entry;
//     struct dirent *entry;

//     // stat for the path
//     stat(path, &stat_path);

//     // if path does not exists or is not dir - exit with status -1
//     if (S_ISDIR(stat_path.st_mode) == 0) {
//         fprintf(stderr, "%s: %s\n", "Is not directory", path);
//         return (2);
//     }

//     // if not possible to read the directory for this user
//     if ((dir = opendir(path)) == NULL) {
//         fprintf(stderr, "%s: %s\n", "Can`t open directory", path);
//         return (2);
//     }

//     // the length of the path
//     path_len = strlen(path);

//     // iteration through entries in the directory
//     while ((entry = readdir(dir)) != NULL) {

//         // skip entries "." and ".."
//         if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
//             continue;

//         // determinate a full path of an entry
//         full_path = (char *)calloc(path_len + strlen(entry->d_name) + 1, sizeof(char));
//         strcpy(full_path, path);
//         strcat(full_path, "/");
//         strcat(full_path, entry->d_name);

//         // stat for the entry
//         stat(full_path, &stat_entry);

//         // recursively remove a nested directory
//         if (S_ISDIR(stat_entry.st_mode) != 0) {
//             if (rmtree(full_path) == 2)
// 				return (2);
// 			continue;
//         }

//         // remove a file object
//         if (unlink(full_path) == 0)
//             printf("Removed a file: %s\n", full_path);
//         else
//             return (2);
//         free(full_path);
//     }
//     // remove the devastated directory and close the object of it
//     if (rmdir(path) == 0)
//         printf("Removed a directory: %s\n", path);
//     else
//         return (2);

//     closedir(dir);
// 	return (1);
// }


// test WorkingLocation::searchLocation()
int main(const int argc, char const *argv[])
{
    // std::vector<location> vec;

    // location test;

    // test.set_locations_path("/dfsd");
    // test.set_root("response/");
    // std::string p = "/s";

    // location test1;
    // test1.set_locations_path("/sll");
    // workingLocation w;
    // vec.push_back(test);
    // vec.push_back(test1);
    // location *l = w.searchLocation(vec, p);
    // if (l)
    // {
    //     std::cout << "ok" << std::endl;
    //     std::cout << l->get_locations_path() << std::endl;
    //     std::cout << "walo" << std::endl;
    // }
    // else
    // {
    //     std::cout << "9WADA" << std::endl;
    // }
    std::string path = "/dsfds/fdffd/";
    // std::cout << path[path.find_last_of("/")] << std::endl;
    int last = path.find_last_of("/");
    std::cout << last << std::endl;

    if (last != -1)
    {
        path.erase(last);
    }
    // std::cout << path << std::endl;
    last = path.find_last_of("/");
    if (last != -1)
        std::cout << path.substr(last + 1) << std::endl;
    return 0;
}