#ifndef COMMAND_COMPLETION_HPP
#define COMMAND_COMPLETION_HPP

#include <iostream>
#include <vector>
#include "builtin.hpp"
#include "parser.hpp"
#include <readline/readline.h>
#include <readline/history.h>
#include "function.hpp"
#include<filesystem>

namespace fs = std::filesystem;



class TabCompletor
{
private:
    static std::vector<std::string> matches;
    static size_t match_index;

    static char *command_generator(const char *text, int state)
    {

        if (state == 0)
        {
            matches.clear();
            std::string prefix(text);

            auto &builtin_list = Builtin<Parser>::getMap();

            for (auto it = builtin_list.begin(); it != builtin_list.end(); ++it)
            {
                if (it->second->get_name().find(prefix) == 0)
                {
                    matches.push_back(it->second->get_name());
                }
            }



            // Tab completion for executable

            std::vector<std::string> directories = getExePath();


            for(auto dir: directories) {

                for(const auto& entry: fs::directory_iterator(dir)) {
                     std::string exe = entry.path().filename().string();

                     if(exe.find(prefix) == 0){
                         matches.push_back(exe);
                     }
                }
            }



            match_index = 0;

            // if(const)
        }

        if (match_index < matches.size())
        {
            return strdup(matches[match_index++].c_str());
        }

        return nullptr;
    }

    TabCompletor();

public:
    static char **my_completion(const char *text, int start, int /*end */)
    {
        if (start == 0)
        {
            return rl_completion_matches(text, command_generator);
        }

        return nullptr;
    }
};

// Define static members
std::vector<std::string> TabCompletor::matches;
size_t TabCompletor::match_index = 0;

#endif