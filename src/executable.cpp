#include "executable.hpp"
#include "redirection.hpp"
#include <fcntl.h>
#include "function.hpp"
#include <algorithm>
#include "parser.hpp"

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



// void Executable::operator()(Parser &ps)
// {
//     auto cmd_queue = ps.get_cmd_args_queue();
//     // std::vector<std::string> argv_strings = total_cmd > 1 ? ps.get_argv_for_mult_cmd()[0] : ps.get_argv();
  

//     int prev_pipe_read = 0; //? Initially STDIN_FILENO
//     int fd[2];              // ? fd[0] for read end & fd[1] for write end

//     while (!cmd_queue.empty())
//     {
//         auto current_cmd = cmd_queue.front();
//         cmd_queue.pop();
//         bool is_last = cmd_queue.empty();
//         if (!is_last)
//         {
//             if (pipe(fd) == -1)
//             {
//                 perror("pipe");
//                 exit(1);
//             }
//         }

//         pid_t pid = fork();

//         if (pid == 0)
//         {
//             // Child process
//             //  * If previos cmd send any output via pipe then read the ouput
//             if (prev_pipe_read != 0)
//             {
//                 dup2(prev_pipe_read, STDIN_FILENO);
//                 close(prev_pipe_read); // close for the child process
//             }

//             if (!is_last)
//             {
//                 close(fd[0]);
//                 dup2(fd[1], STDOUT_FILENO); // ** Write the output in pipe  current pipe write end not to the stdout
//                 close(fd[1]);
//             }
           

//         //    // Print debug information
//         //     std::cout << "=== Child Process " << getpid() << " ===" << std::endl;
//         //     std::cout << "Command: " << current_cmd.cmd << std::endl;
//         //     std::cout << "Arguments: ";
//         //     for (auto x : current_cmd.argv) {
//         //         std::cout << x << " ";
//         //     }
//         //     std::cout << std::endl;
            
//         //     // Build argument vector
//         //     std::vector<char*> v = exec_vector(current_cmd.cmd, current_cmd.argv);
            
//         //     std::cout << "Arg vector: ";
//         //     for (size_t i = 0; v[i] != nullptr; i++) {
//         //         std::cout << v[i] << " ";
//         //     }
//         //     std::cout << "(null terminated)" << std::endl;
            
//         //     std::cout << "Pipe setup: " << std::endl;
//         //     std::cout << "  prev_pipe_read: " << prev_pipe_read << std::endl;
//         //     std::cout << "  is_last: " << (is_last ? "true" : "false") << std::endl;
//         //     if (!is_last) {
//         //         std::cout << "  fd[0] (read end): " << fd[0] << std::endl;
//         //         std::cout << "  fd[1] (write end): " << fd[1] << std::endl;
//         //     }
            
//         //     std::cout << "=========================" << std::endl;
//         //     std::cout.flush();  // Force flush output
            
//         //     // IMPORTANT: Exit child process after printing
//         //     // Don't call execvp() - just exit
//         //     exit(0);
            


//             execvp(current_cmd.cmd.c_str(), exec_vector(current_cmd.cmd, current_cmd.argv).data());
//         }
//         else
//         {
//             // parent process

//             //  *  close the read end the previos pipe if exist

//             if (prev_pipe_read != 0)
//             {
//                 close(prev_pipe_read);
//             }

//             if (!is_last)
//             {
//                 close(fd[1]);
//                 prev_pipe_read = fd[0];
//             }
//         }
//     }
    
//     while(wait(nullptr) > 0);

  
// }