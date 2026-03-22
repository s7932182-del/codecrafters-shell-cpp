#include "parser.hpp"

Parser::Parser(const std::string input)
{
    std::stringstream ss(input);
    std::string word;
    bool command_is_appended = false;

    while(ss >> word) {
        if(!command_is_appended) {
             this->command = word;
             command_is_appended = true;
        } 
             this->argv.push_back(word);
        }
}

std::string Parser::get_command() {
     return this->command;
}

std::vector<std::string>& Parser::get_argv() {
     return this->argv;
}

void Parser::print_arg() {
    for(int i = 1 ; i < this->argv.size(); i++) {
         std::cout << argv[i] << " ";
    }
    std::cout << std::endl;
}
