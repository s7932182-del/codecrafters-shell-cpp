#include "function.hpp"
#include <iostream>
#include <vector>
#include <sstream>

std::vector<std::string> getExePath()
{
    const char *PATH_VARIABLE = std::getenv("PATH");

    if (PATH_VARIABLE == nullptr)
        std::cerr << "Path variable is not found" << std::endl;

    std::stringstream ss(PATH_VARIABLE);
    std::vector<std::string> directories;
    std::string dir;

#ifdef _WIN32
    char delimiter = ';';
#else
    char delimiter = ':';
#endif

    while (std::getline(ss, dir, delimiter))
    {
        if (!dir.empty())
        {
            directories.push_back(dir);
        }
    }

    return directories;
}
