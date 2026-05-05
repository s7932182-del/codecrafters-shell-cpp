#ifndef COMMAND_EXECUTER_HPP
#define COMMAND_EXECUTER_HPP

#include "parser.hpp"

class CommandExecutor
{
public:
    static void execute(const Parser &parser);

private:
    static std::vector<char *> exec_vector( std::vector<std::string> &argv);
};

#endif