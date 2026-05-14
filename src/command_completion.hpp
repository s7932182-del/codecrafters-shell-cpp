#ifndef COMMAND_COMPLETION_HPP
#define COMMAND_COMPLETION_HPP

#include <complex>
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

    static char *argument_generator(const char *text, const int state) {
        if (state == 0) {
            matches.clear();
            match_index = 0;

            const int end = rl_end;
            const char *line = rl_line_buffer;
            std::string arg1 = "";
            std::string arg3 = "";
            std::string arg2(text);
            std::string cmd;

            std::istringstream input_str(line);
            std::vector<std::string> words;
            std::string word;
            while (input_str >> word) {
                words.push_back(word);
            }

            cmd = arg1 = std::string(words[0]);
            arg2 = text;
            if (words.size() == 3) arg3 = words[1];


            if (!cmd.empty()) {
                auto it = COMPLETE::script_list.find(cmd);
                if (it != COMPLETE::script_list.end()) {
                    const std::string exe = it->second;
                    int pipeFd[2];
                    pipe(pipeFd);
                    const pid_t pid = fork();
                    if (pid == 0) {

                        setenv("COMP_LINE", line, 1);
                        setenv("COMP_POINT", std::to_string(rl_point).c_str(), 1);
                        close(pipeFd[0]);
                        dup2(pipeFd[1], STDOUT_FILENO);
                        close(pipeFd[1]);
                        execl(exe.c_str(), exe.c_str(), arg1.c_str(), arg2.c_str(), arg3.c_str(), nullptr);
                        perror("execl");
                        exit(1);
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
            const std::string &match = matches[match_index++];
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
            const char *line = rl_line_buffer;

            std::string cmd;
            for (int i = 0; i < end; i++) {
                if (line[i] == ' ') {
                    cmd = std::string(line, i);
                    break;
                }
            }
            std::string cmd_spaces = cmd + ' ';

            auto it = COMPLETE::script_list.find(cmd);
            if (it != COMPLETE::script_list.end()) {
                // Use the custom generator for this command
                std::string first = it->first + " ";
                if (cmd_spaces == first)
                    return rl_completion_matches(text, argument_generator);
            }


            // Fallback to filename completion
            return rl_completion_matches(text, rl_filename_completion_function);
        }
    }
};

// Define static members
std::vector<std::string> TabCompletor::matches;
size_t TabCompletor::match_index = 0;

#endif
