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
    std::string cmd_string;
    
    public:
    Parser(const std::string& input);
    // std::vector<std::string> get_command();
    // std::vector<std::string> get_argv();

   

    [[nodiscard]] std::string get_output_file() const;
    [[nodiscard]] std::string get_error_file() const;
    [[nodiscard]] bool has_output_redirect() const;
    [[nodiscard]] bool has_error_redirect() const;
    [[nodiscard]] bool is_append_mode() const;
    // std::vector<std::vector<std::string>> get_argv_for_mult_cmd() const;
    [[nodiscard]] std::queue<Cmd> get_cmd_args_queue() const;
    [[nodiscard]] bool get_valid_cmd() const;
    [[nodiscard]] bool get_is_exit() const;
    [[nodiscard]] std::string get_cmd_string() const;
    // [[nodiscard]] std::string get_cmd_name() const;
    static std::string resolve_dollar(std::string argument);
};

#endif
