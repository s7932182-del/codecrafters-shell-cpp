#include "executable.hpp"

std::vector<std::string> Executable::getEnvironmentVariable()
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

Executable::Executable(std::string command) : exe(command)
{
    std::vector<std::string> directories = getEnvironmentVariable();

    for (auto dir : directories)
    {
        fs::path fullPath = fs::path(dir) / this->exe;
        if (fs::exists(fullPath))
        {
            auto file_perms = fs::status(fullPath).permissions();
            fs::perms exec_perm = fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec;
            if ((file_perms & exec_perm) != fs::perms::none)
            {
                this->is_exe = true;
                this->exe_path = fullPath.string();
                break;
            }
        }
    }
}

std::string Executable::get_path()
{
    return this->exe_path;
}

bool Executable::operator()()
{
    return this->is_exe;
}

void Executable::operator()(const std::vector<std::string> &argv)
{
    std::vector<char *> args;
    std::vector<std::vector<char>> arg_storage;

    args.reserve(argv.size() + 1);
    arg_storage.reserve(argv.size());

    for (const auto &arg : argv)
    {
        std::vector<char> mutable_arg(arg.begin(), arg.end());
        mutable_arg.push_back('\0');
        arg_storage.push_back(std::move(mutable_arg));
        args.push_back(arg_storage.back().data());
    }
    args.push_back(nullptr);

    pid_t pid = fork();

    if (pid == 0)
    {
        if (execv(this->exe_path.c_str(), args.data()) == -1)
        {
            std::cerr << "Error occurred while exec" << std::endl;
        }
    }
    else
    {
        wait(NULL);
    }
}