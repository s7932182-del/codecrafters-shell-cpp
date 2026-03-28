#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <queue>

class Parser
{
private:
    // std::vector<std::string> command;
    // std::vector<std::vector<std::string>> argv_for_mult_cmd;
    // std::vector<std::string> argv;
    

     struct Cmd
    {
        std::string cmd;
        std::vector<std::string> argv;
        bool is_builtin;
    };

     std::queue<Cmd> q;
    
   
    std::string output_file;
    std::string error_file;
    bool has_output_redirection;
    bool has_error_redirection;
    bool has_append_mode;
    bool is_new_cmd;
    bool is_valid_cmd;
    bool isExit;
    
    public:
    Parser(const std::string input);
    // std::vector<std::string> get_command();
    // std::vector<std::string> get_argv();

   

    std::string get_output_file() const;
    std::string get_error_file() const;
    bool has_output_redirect() const;
    bool has_error_redirect() const;
    bool is_append_mode() const;
    // std::vector<std::vector<std::string>> get_argv_for_mult_cmd() const;
    std::queue<Cmd> get_cmd_args_queue() const;
    bool get_valid_cmd() const;
    bool get_is_exit() const;
};

#endif
