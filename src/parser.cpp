#include "parser.hpp"
#include <vector>
#include <algorithm>
#include "redirection.hpp"
#include <string>

Parser::Parser(const std::string input)
{

    // std::cout << "Input: " << input << std::endl;

    int st = 0, end = input.length() - 1;
    bool is_command = false;
    this->has_output_redirection = false;
    this->has_error_redirection = false;

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
                if(input[st-1] =='1' || isspace(input[st-1])){
                    if(!isspace(input[st-1])) argument.pop_back();
                    this->has_output_redirection = true;
                } else if(input[st-1] == '2') {
                     argument.pop_back();
                     this->has_error_redirection = true;
                     
                }

                st++;
            }
            else
            {
                if (!isspace(input[st]))
                    argument.push_back(input[st]);
                st++;
            }
        }

        if (!is_command)
        {
            this->command = argument;
            is_command = false;
        }
        is_command = true;
        if (!argument.empty())
        {
             if(this->has_output_redirect()) {
                 this->output_file = argument;
             }else if(this->has_error_redirect()) {
                 this->error_file = argument;
             }else {
                 this->argv.push_back(argument);
             }
        }
        // st++;
    }

}

std::string Parser::get_command()
{
    return this->command;
}

std::vector<std::string> &Parser::get_argv()
{
    return this->argv;
}

void Parser::print_arg()
{
    for (size_t i = 1; i < this->argv.size(); i++)
    {
        std::cout << argv[i] << " ";
    }
    std::cout << std::endl;
}

std::string Parser::get_output_file () const
{
    return this->output_file;
}


bool Parser::has_output_redirect() const {
   return this->has_output_redirection;
}



bool Parser::has_error_redirect() const {
     return this->has_error_redirection;
}


std::string Parser::get_error_file() const {
     return this->error_file;
}
