#include "parser.hpp"
#include <vector>
#include <algorithm>
#include "redirection.hpp"
#include "builtin.hpp"
#include <string>

Parser::Parser(const std::string input)
{

    // std::cout << "Input: " << input << std::endl;

    int st = 0, end = input.length() - 1;
    bool is_command = false;
    this->has_output_redirection = false;
    this->has_error_redirection = false;
    this->has_append_mode = false;
    this->is_new_cmd = true;
    Cmd current_cmd;
    this->is_new_cmd = true;
    isExit =false;
    is_valid_cmd = true;

    while (st <= end)
    {
        while (isspace(input[st]) && st <= end)
            st++;
        std::string argument;

        while (!isspace(input[st]) && st <= end)
        {

            if (input[st] == '\'')
            {
                st++;
                while (input[st] != '\'')
                {

                    argument.push_back(input[st]);
                    st++;
                }
                st++;
            }
            else if (input[st] == '\"')
            {

                st++;
                while (input[st] != '\"')
                {
                    argument.push_back(input[st]);
                    st++;

                    if (input[st] == '\\')
                    {
                        st++;
                        argument.push_back(input[st]);
                        st++;
                    }
                }
                st++;
            }
            else if (input[st] == '\\')
            {
                // argument.push_back(input[st+1]);
                st++;
                if (st <= end)
                {
                    argument.push_back(input[st]);
                    st++;
                }
            }
            else if (input[st] == '>')
            {
                if (input[st - 1] == '1' || isspace(input[st - 1]))
                {
                    if (!isspace(input[st - 1]))
                        argument.pop_back();
                    this->has_output_redirection = true;
                    if (input[st + 1] == '>')
                    {
                        this->has_append_mode = true;
                        st++;
                    }
                }
                else if (input[st - 1] == '2')
                {
                    argument.pop_back();
                    this->has_error_redirection = true;
                    if (input[st + 1] == '>')
                    {
                        this->has_append_mode = true;
                        st++;
                    }
                }

                st++;
            }
            else if (input[st] == '|')
            {
                // this->argv_for_mult_cmd.push_back(this->argv);
                // this->argv.clear();
                // is_command = false;
                st++;

                if (!current_cmd.cmd.empty() || !current_cmd.argv.empty())
                {
                    q.push(current_cmd);
                }
                current_cmd = Cmd();
                is_command = false;

                break;
            }
            else
            {
                if (!isspace(input[st]))
                    argument.push_back(input[st]);
                st++;
            }
        }

        if (!is_command && !argument.empty())
        {

            // cmd.cmd = argument;
            // this->command.push_back(argument);

            // Check for builtin commmand

            if(argument == "exit") {
                this->isExit = true;
                break;
            }

            auto builtin_cmd = Builtin<Parser>::getMap();

            if (builtin_cmd.count(argument))
                current_cmd.is_builtin = true;

            //  Check for executable
            else
            {
                Executable exe(argument);

                if (exe())
                    current_cmd.is_builtin = false;
                else
                {
                    this->is_valid_cmd = false;
                    std::cout << input << ": command not found" << std::endl;
                    break;
                }
            }

            current_cmd.cmd = argument;
            is_command = true;
        }

        if (!argument.empty())
        {
            if (this->has_output_redirect())
            {
                this->output_file = argument;
            }
            else if (this->has_error_redirect())
            {
                this->error_file = argument;
            }
            else
            {

                // this->argv.push_back(argument);
                current_cmd.argv.push_back(argument);
            }
        }
        // st++;
    }

    // Push the last command if it exists
    if (!current_cmd.cmd.empty() || !current_cmd.argv.empty())
    {
        q.push(current_cmd);
    }
}

std::string Parser::get_output_file() const
{
    return this->output_file;
}

bool Parser::has_output_redirect() const
{
    return this->has_output_redirection;
}

bool Parser::has_error_redirect() const
{
    return this->has_error_redirection;
}

std::string Parser::get_error_file() const
{
    return this->error_file;
}

bool Parser::is_append_mode() const
{
    return this->has_append_mode;
}


bool Parser::get_valid_cmd() const
{
    return this->is_valid_cmd;
}


bool Parser::get_is_exit() const
{
    return this->isExit;
}

std::queue<Parser::Cmd> Parser::get_cmd_args_queue() const
{
    return this->q;
}
