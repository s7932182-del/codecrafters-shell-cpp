#include "executable.hpp"
#include "redirection.hpp"
#include <fcntl.h>
#include "function.hpp"
#include <algorithm>

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
    int total_cmd = ps.get_command().size();
    std::vector<std::string> argv_strings = total_cmd > 1 ? ps.get_argv_for_mult_cmd()[0] : ps.get_argv();
    std::vector<char *> argv;

    for (auto &s : argv_strings)
    {
        argv.push_back(s.data()); // C++17 gives non-const pointer
    }
    argv.push_back(nullptr);

    int fd[2];

    if (total_cmd > 1)
    {
        pipe(fd);
    }

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

        if (total_cmd > 1)
        {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
        }

        if (execv(this->exe_path.c_str(), argv.data()) == -1)
        {
            std::cerr << "Error occurred while exec" << std::endl;
            exit(1);
        }
    }

    pid_t pid2;

    if (total_cmd > 1)
    {
        std::string cmd = ps.get_command()[1];

        auto args_for_this_cmd = ps.get_argv();
        std::vector<char *> argv_for_this_cmd;

        if (!args_for_this_cmd.empty())
        {

            for (auto &s : args_for_this_cmd)
            {
                argv_for_this_cmd.push_back(s.data()); // C++17 gives non-const pointer
            }
            argv_for_this_cmd.push_back(nullptr);
        }

        pid2 = fork();

        if(pid2 == 0) {

            close(fd[1]);
    
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
    
            execvp(cmd.c_str(), argv_for_this_cmd.data());
        }


        //  pid2 = fork();
    }

    if (total_cmd > 1)
    {
        close(fd[0]);
        close(fd[1]);
    }

    // std::cout << "Parent is running " << std::endl;

    waitpid(pid, NULL,0);
    waitpid(pid2, NULL, 0);
}