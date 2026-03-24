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
    std::string output_file;
    std::string error_file;
    bool has_output_redirection;
    bool has_error_redirection;
    bool has_append_mode;

public:
    Parser(const std::string input);
    std::string get_command();
    std::vector<std::string> &get_argv();
    void print_arg();
    std::string get_file();

     std::string get_output_file() const;
    std::string get_error_file() const;
    bool has_output_redirect() const;
    bool has_error_redirect() const;
    bool is_append_mode() const;
};

#endif
