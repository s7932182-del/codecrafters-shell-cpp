#include "executable.hpp"
#include "redirection.hpp"
#include <fcntl.h>
#include "function.hpp"



Executable::Executable(std::string command) : exe(command)
{
    std::vector<std::string> directories = getExePath();

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

void Executable::operator()(Parser &ps)
{
    std::vector<char *> args;
    std::vector<std::vector<char>> arg_storage;

    args.reserve(ps.get_argv().size() + 1);
    arg_storage.reserve(ps.get_argv().size());

    for (const auto &arg : ps.get_argv())
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

        // / Use Redirection class in child process

        if (ps.has_output_redirect())
        {
            // Create redirection object - this will redirect cout in the child

            int output_file;

            if (ps.is_append_mode())
            {
                output_file = open(ps.get_output_file().c_str(), O_WRONLY | O_CREAT | O_APPEND, 0777);
            }
            else
            {

                output_file = open(ps.get_output_file().c_str(), O_WRONLY | O_CREAT, 0777);
            }

            dup2(output_file, STDOUT_FILENO);

            close(output_file);
        }

        if (ps.has_error_redirect())
        {
            int error_file;

            if (ps.is_append_mode())
            {
                error_file = open(ps.get_error_file().c_str(), O_WRONLY | O_CREAT | O_APPEND, 0777);
            }
            else
            {

                error_file = open(ps.get_error_file().c_str(), O_WRONLY | O_CREAT, 0777);
            }
            dup2(error_file, STDERR_FILENO);
            close(error_file);
        }

        // Execute the command
        if (execv(this->exe_path.c_str(), args.data()) == -1)
        {
            std::cerr << "Error occurred while exec" << std::endl;
            exit(1);
        }
    }
    else
    {
        wait(NULL);
    }
}