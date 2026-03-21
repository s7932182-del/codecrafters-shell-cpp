#include <iostream>
#include <vector>
#include<filesystem>
#include <sstream>


namespace fs = std::filesystem;

class Executable
{

    std::string exe;
    std::string exePath;

    std::vector<std::string> getEnvironmentVariable()
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

public:
    Executable(std::string command) : exe(command) {}

    std::string operator()() {
        std::vector<std::string> directories = getEnvironmentVariable();

       for(auto dir: directories) {
           fs::path fullPath  = fs::path(dir) / this->exe;
           if(fs::exists(fullPath)) {
              auto perms = fs::status(fullPath).permissions();
              if((perms & fs::perms::owner_exec)  != fs::perms::none) {
                  return fullPath;
              }
           }
       }


       return "";
    }

};