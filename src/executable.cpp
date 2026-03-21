#include <iostream>
#include <vector>
#include <filesystem>
#include <sstream>
#include <unistd.h>


namespace fs = std::filesystem;

class Executable
{

    std::string exe;
    bool is_exe = false;
    std::string exe_path = "";

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
    Executable(std::string command) : exe(command)
    {

        std::vector<std::string> directories = getEnvironmentVariable();

        for (auto dir : directories)
        {
            fs::path fullPath = fs::path(dir) / this->exe;
            if (fs::exists(fullPath))
            {
                auto perms = fs::status(fullPath).permissions();
                if ((perms & fs::perms::owner_exec) != fs::perms::none)
                {
                    this->is_exe = true;
                    this->exe_path = fullPath.string();
                }
            }
        }
    }

    std::string get_path()
    {
        return this->exe_path;
    }

    bool operator()()
    {
        return this->is_exe;
    }

    void operator()(const std::vector<std::string> &argv)
    {
        // std::vector<char*> args;
        // for (const auto& arg : argv) {
        //     args.push_back(const_cast<char*>(arg.c_str()));
        // }
        // args.push_back(nullptr);
        // execv(this->exe_path.c_str(), args.data());

        // execv()  //int execv(const char *__path, char *const __argv[])

        std::vector<char *> args;
        std::vector<std::vector<char>> arg_storage; // Store mutable copies

        // Reserve space for efficiency
        args.reserve(argv.size() + 1);
        arg_storage.reserve(argv.size());

        for (const auto &arg : argv)
        {
            // Create mutable copy
            std::vector<char> mutable_arg(arg.begin(), arg.end());
            mutable_arg.push_back('\0'); // Null terminate
            arg_storage.push_back(std::move(mutable_arg));
            args.push_back(arg_storage.back().data());
        }
        args.push_back(nullptr);

        // Execute
        if (execv(this->exe_path.c_str(), args.data()) == -1)
        {
            // Handle error
            std::cerr << "Error occoured while exec" << std::endl;
        }
    }
};