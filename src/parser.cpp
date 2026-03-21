#include <iostream>
#include<vector>
#include<sstream>

class Parser
{
    std::string command;
    std::vector<std::string> argv;


public:
    Parser(const std::string input)
    {
        std::stringstream ss(input);
        std::string word;
        bool command_is_appended =false;

        while(ss >> word) {
            if(!command_is_appended) {
                 this->command = word;
                 command_is_appended =true;
            } 
                 this->argv.push_back(word);
            }
    }

    std::string get_command() {
         return this->command;
    }

    std::vector<std::string>& get_argv() {
         return this->argv;
    }

    void print_arg() {
        for(auto arg: this->argv) {
            std::cout << arg << " ";
        }
        std::cout << std::endl;
    }
};
