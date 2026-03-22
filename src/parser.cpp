#include "parser.hpp"

Parser::Parser(const std::string input)
{

    // std::cout << "Input: " << input << std::endl;

    int st = 0, end = input.length() - 1;
    bool is_command = false;

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

                    if (input[st] == '//')
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
            else
            {

                if (!is_command)
                {
                    this->command.push_back(input[st]);
                }

                argument.push_back(input[st]);
                st++;
            }
        }
        is_command = true;
        this->argv.push_back(argument);
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
