#ifndef COMMAND_EXECUTER_HPP
#define COMMAND_EXECUTER_HPP

#include "parser.hpp"

class CommandExecutor
{
public:
    static void execute(Parser &parser);

private:
    static std::vector<char *> exec_vector( std::vector<std::string> &argv);
    // static bool is_builtin(const std::string& cmd);
    // static void execute_builtin(Parser& parser, const std::string& cmd);
    // static void execute_external(const std::string& cmd, const std::vector<std::string>& args);
    // static void execute_pipeline(Parser& parser);
};

#endif