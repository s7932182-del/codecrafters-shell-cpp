#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include <sstream>

class Parser
{
private:
    std::string command;
    std::vector<std::string> argv;

public:
    Parser(const std::string input);
    std::string get_command();
    std::vector<std::string>& get_argv();
    void print_arg();
};

#endif
