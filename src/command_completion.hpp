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


class TabCompletor {
private:
    static std::vector<std::string> matches;
    static size_t match_index;

    static char *command_generator(const char *text, const int state) {
        if (state == 0) {
            matches.clear();
            const std::string prefix(text);

            auto &builtin_list = Builtin<Parser>::getMap(); // Return the builtin commadn

            for (auto &it: builtin_list) {
                if (it.second->get_name().find(prefix) == 0) {
                    matches.push_back(it.second->get_name());
                }
            }


            // Tab completion for executable

            std::vector<std::string> directories = getExePath(); // Return the path of all executable
            for (const auto &dir: directories) {
                for (const auto &entry: fs::directory_iterator(dir)) {
                    std::string exe = entry.path().filename().string();

                    if (exe.find(prefix) == 0) {
                        matches.push_back(exe);
                    }
                }
            }


            match_index = 0;

            // if(const)
        }

        if (match_index < matches.size()) {
            return strdup(matches[match_index++].c_str());
        }

        return nullptr;
    }
   static char * argument_generator(const char *text, const int state) {
        if (state == 0) {
            matches.clear();

            const int cursor_at  = rl_point;
            const char * line = rl_line_buffer;
            std::string cmd;

            for (int  i = 0; i<cursor_at; i++) {
                if (line[i] == ' ') {
                    cmd = std::string(line,i);
                    break;
                }
            }

            if (!cmd.empty()) {
                auto it  = COMPLETE::script_list.find(cmd);
                if (it != COMPLETE::script_list.end()) {
                    const std::string exe = it->second;
                    int pipeFd[2];
                    pipe(pipeFd);
                    const pid_t pid = fork();
                    if (pid == 0) {
                        close(pipeFd[0]);
                        dup2(pipeFd[1], STDOUT_FILENO);
                        close(pipeFd[1]);
                        execl(cmd.c_str(), exe.c_str(), nullptr);
                    } else {
                        close(pipeFd[1]);
                        std::string output;
                        char buffer[1024];
                        ssize_t bytes;
                        while ((bytes = read(pipeFd[0], buffer, sizeof(buffer))) > 0) {
                            buffer[bytes] = '\0';
                            output += buffer;
                        }
                       close(pipeFd[0]);
                        wait(nullptr);
                        std::istringstream iss(output);

                        std::string match;
                        while (iss >> match) {
                            matches.push_back(match);
                        }

                    }
                }
            }
        }

        // Return matches that match current text
        while (match_index < matches.size()) {
            const std::string& match = matches[match_index++];
            if (match.compare(0, strlen(text), text) == 0) {
                return strdup(match.c_str());
            }
        }
        return nullptr;

    }
public:
    TabCompletor() = delete;

    static char **my_completion(const char *text, const int start, const int end) {
        if (start == 0)
            return rl_completion_matches(text, command_generator);
        else {
            const char* line = rl_line_buffer;
            const std::string cmd(line, start - 1);
            const std::string prev_word(line, start);

            if (prev_word == (cmd + ' ')) {
                auto it = COMPLETE::script_list.find(cmd);
                if (it != COMPLETE::script_list.end()) {
                    // Use the custom generator for this command
                    return rl_completion_matches(text, argument_generator);
                }
            }

            // Fallback to filename completion
            return rl_completion_matches(text, rl_filename_completion_function);
        }
        return  nullptr;
    }
};

// Define static members
std::vector<std::string> TabCompletor::matches;
size_t TabCompletor::match_index = 0;

#endif
