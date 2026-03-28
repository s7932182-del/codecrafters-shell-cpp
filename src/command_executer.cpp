#include "command_executer.hpp"
#include "parser.hpp"
#include "builtin.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

std::vector<char *> CommandExecutor::exec_vector(std::vector<std::string> &args)
{

    std::vector<char *> argv;

    for (auto &s : args)
    {
        argv.push_back(s.data()); // C++17 gives non-const pointer
    }
    argv.push_back(nullptr);

    return argv;
}

void CommandExecutor::execute(Parser &ps)
{

    auto cmd_queue = ps.get_cmd_args_queue();

    // TODO:

    //  * execute single command and  handle redirection

    if (cmd_queue.size() == 1)
    {
        auto current_cmd = cmd_queue.front();
        cmd_queue.pop();

        //  * * If Error redirection avilable

        if (current_cmd.is_builtin)
        {
            //  TODO: If Redirection is available in builtin command then redirect via pipe

            int saved_stdout = -1;
            int saved_stderr = -1;

            if (ps.has_output_redirect())
            {
                saved_stdout = dup(STDOUT_FILENO);
                int output_file = open(ps.get_output_file().c_str(),
                                       O_WRONLY | O_CREAT | (ps.is_append_mode() ? O_APPEND : O_TRUNC), 0777);
                dup2(output_file, STDOUT_FILENO);
                close(output_file);
            }

            if (ps.has_error_redirect())
            {
                saved_stderr = dup(STDERR_FILENO);
                int error_file = open(ps.get_error_file().c_str(),
                                      O_WRONLY | O_CREAT | (ps.is_append_mode() ? O_APPEND : O_TRUNC), 0777);
                dup2(error_file, STDERR_FILENO);
                close(error_file);
            }

            auto builtin_cmd = Builtin<Parser>::getMap()[current_cmd.cmd];
            builtin_cmd->execute(current_cmd.argv);

            // Restore original file descriptors
            if (saved_stdout != -1)
            {
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdout);
            }
            if (saved_stderr != -1)
            {
                dup2(saved_stderr, STDERR_FILENO);
                close(saved_stderr);
            }
        }
        else
        {
            pid_t pid = fork();

            if (pid == 0)
            {

                // ? child process

                // / Setup redirections in child process only
                if (ps.has_output_redirect())
                {
                    int output_file = open(ps.get_output_file().c_str(),
                                           O_WRONLY | O_CREAT | (ps.is_append_mode() ? O_APPEND : O_TRUNC), 0777);
                    if (output_file == -1)
                    {
                        perror("open output file");
                        exit(EXIT_FAILURE);
                    }
                    dup2(output_file, STDOUT_FILENO);
                    close(output_file);
                }

                if (ps.has_error_redirect())
                {
                    int error_file = open(ps.get_error_file().c_str(),
                                          O_WRONLY | O_CREAT | (ps.is_append_mode() ? O_APPEND : O_TRUNC), 0777);
                    if (error_file == -1)
                    {
                        perror("open error file");
                        exit(EXIT_FAILURE);
                    }
                    dup2(error_file, STDERR_FILENO);
                    close(error_file);
                }
                execvp(current_cmd.cmd.c_str(), exec_vector(current_cmd.argv).data());
            }

            waitpid(pid, NULL, 0);
        }
    }

    else
    {

        //     For multiple pipeline

        int fd[2];

        int prev_pipe_read_end = 0;
        std::vector<pid_t> child_pids;

        // int original_stdin = dup(STDIN_FILENO);
        // int original_stdout = dup(STDOUT_FILENO);
        // Flag to determine if we need to fork built-ins in pipeline
        // Since we have multiple commands, we should fork ALL commands
      

        /*

             TODO_1: let do this with  first example : pwd | grep "pwd"
             1. parent writes on the pipe write end and store the read end for the next commadn
             2. First step output of pwd is written in write end of the pipe and store  its read  end for the next command
             3. Then for a grep which is not a builtin command.
             4. It reads fron the read end of the previoss pipe, execute accroding them -> ouput is written current pipe write end and store the read end in parent processs for next commaand

        */

        while (!cmd_queue.empty())
        {
            auto current_cmd = cmd_queue.front();
            cmd_queue.pop();
            bool is_last = cmd_queue.empty();

            if (!is_last)
            {
                if (pipe(fd) == -1)
                {
                    std::cerr << "Pipe Errror " << std::endl ;
                    exit(1);
                }
            }

           
              // For pipelines, we fork for ALL commands (including built-ins)
            // This is simpler and more reliable
            pid_t pid = fork();
        
            if(pid == 0) {
                // Setup input from previous pipe
                if (prev_pipe_read_end != 0)
                {
                    dup2(prev_pipe_read_end, STDIN_FILENO);
                    close(prev_pipe_read_end);
                }

              
                 // Setup output to next pipe (if not last)
                if (!is_last)
                {
                    close(fd[0]);  // Close read end in child
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[1]);  // Close write end after dup2
                }

                // Handle redirections for the pipeline
                // Only apply output redirection to the last command
                if (is_last && ps.has_output_redirect())
                {
                    int out_fd = open(ps.get_output_file().c_str(),
                                     O_WRONLY | O_CREAT | (ps.is_append_mode() ? O_APPEND : O_TRUNC),
                                     0777);
                    if (out_fd != -1)
                    {
                        dup2(out_fd, STDOUT_FILENO);
                        close(out_fd);
                    }
                }

                // Apply error redirection to all commands in pipeline
                if (ps.has_error_redirect())
                {
                    int err_fd = open(ps.get_error_file().c_str(),
                                     O_WRONLY | O_CREAT | (ps.is_append_mode() ? O_APPEND : O_TRUNC),
                                     0777);
                    if (err_fd != -1)
                    {
                        dup2(err_fd, STDERR_FILENO);
                        close(err_fd);
                    }
                }


                 if (current_cmd.is_builtin)
                {
                    auto builtin_cmd = Builtin<Parser>::getMap()[current_cmd.cmd];
                    builtin_cmd->execute(current_cmd.argv);
                    // fflush(stdout);
                    // fflush(stderr);
                    exit(0);  // Important: Exit child after built-in execution
                }
                else
                {
                    execvp(current_cmd.cmd.c_str(), exec_vector(current_cmd.argv).data());
                     std::cerr << "Error in execvp" << std::endl;
                    exit(1);
                }

            } else if(pid > 0) {
                child_pids.push_back(pid);
                
                // Close previous pipe read end in parent
                if (prev_pipe_read_end != 0)
                {
                    close(prev_pipe_read_end);
                }


                  // Setup for next command
                if (!is_last)
                {
                    close(fd[1]);  // Close write end in parent
                    prev_pipe_read_end = fd[0];  // Save read end for next command
                }
                else
                {
                    // Last command - close any remaining pipe read end
                    if (prev_pipe_read_end != 0)
                    {
                        // close(prev_pipe_read_end);
                        prev_pipe_read_end = 0;
                    }
                }
            } else {
                std::cerr<< "Error in fork" << std::endl;
                exit(1);
            }
        

        
        }

        
        

        for (pid_t pid : child_pids)
        {
            int status;
            waitpid(pid, &status, 0);
        }


        // Restore original stdin/stdout
        // dup2(original_stdin, STDIN_FILENO);
        // dup2(original_stdout, STDOUT_FILENO);
        // close(original_stdin);
        // close(original_stdout);
    }
}