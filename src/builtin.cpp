#include "builtin.hpp"
#include <filesystem>
#include "parser.hpp"
#include <readline/history.h>
#include "backgroundJob.hpp"
#include<iomanip>
#include <string_view>
#include <ranges>

namespace fs = std::filesystem;

// TYPE class implementation
TYPE::TYPE() : Builtin("type") {
}

TYPE &TYPE::getInstance() {
    static TYPE instance;
    return instance;
}

void TYPE::execute(const std::vector<std::string> &args) {
    const auto &builtinMap = Builtin::getMap();
    const auto &arg = args[1];

    if (builtinMap.count(arg)) {
        std::cout << arg << " is a shell builtin" << std::endl;
        return;
    }

    Executable exe(arg);
    if (exe()) {
        std::cout << arg << " is " << exe.get_path() << std::endl;
    } else {
        std::cout << arg << ": not found" << std::endl;
    }
}

std::string TYPE::get_name() {
    return this->name;
}

// ECHO class implementation
ECHO::ECHO() : Builtin("echo") {
}

ECHO &ECHO::getInstance() {
    static ECHO instance;
    return instance;
}

void ECHO::execute(const std::vector<std::string> &args) {
    size_t i = 1;

    for (i = 1; i < args.size() - 1; i++) {
        std::cout << args[i] << " ";
    }

    std::cout << args[i];
    std::cout << std::endl;

    // return 1;
}

std::string ECHO::get_name() {
    return this->name;
}

// EXIT class implementation
EXIT::EXIT() : Builtin("exit") {
}

EXIT &EXIT::getInstance() {
    static EXIT instance;
    return instance;
}

void EXIT::execute(const std::vector<std::string> &) {
    // ps.get_cmd_args_queue().pop();
    // return 0;
}

std::string EXIT::get_name() {
    return this->name;
}

//  PWD class Implementation

PWD::PWD() : Builtin("pwd") {
};

std::string PWD::get_name() {
    return this->name;
}

PWD &PWD::getInstance() {
    static PWD instance;
    return instance;
}

void PWD::execute(const std::vector<std::string> &) {
    // ps.get_cmd_args_queue().pop();
    const std::string cwd = fs::current_path();
    std::cout << cwd << std::endl;
}

//  CD Class Implementation

CD::CD() : Builtin("cd") {
};

CD &CD::getInstance() {
    static CD instance;
    return instance;
}

void CD::execute(const std::vector<std::string> &args) {
    // std::string dir = ps.get_argv()[1];

    std::string dir = args[1];
    if (dir == "~") {
        const char *home = getenv("HOME");
        dir = home;
    }
    try {
        fs::current_path(dir);
    } catch (const fs::filesystem_error &e) {
        // std::cerr << "  What: " << e.what() << "\n";
        // std::cerr << "  Path1: " << e.path1() << "\n";

        if (e.code() == std::errc::no_such_file_or_directory) {
            std::cerr << "cd: " << dir << ": No such file or directory" << std::endl;
        } else if (e.code() == std::errc::permission_denied) {
            std::cerr << "  Reason: Permission denied\n";
        }
    }
}

std::string CD::get_name() {
    return this->name;
}

//  HISTORY Class Implementation

HISTORY::HISTORY() : Builtin("history") {
};

HISTORY &HISTORY::getInstance() {
    static HISTORY instance;
    return instance;
}

void HISTORY::execute(const std::vector<std::string> &args) {
    static int count = 0;

    const char *histFile = getenv("HISTFILE");
    if (args[1] == "-r") {
        read_history(args[2].c_str());
        return;
    } else if (args[1] == "-w") {
        const char *file;
        if (args.size() == 2) {
            file = histFile;
        } else {
            file = args[2].c_str();
        }
        write_history(file);
        return;
    } else if (args[1] == "-a") {
        int total = 0;
        HIST_ENTRY **history = history_list();

        while (history[total++] != nullptr);
        count = total - count;
        append_history(count, args[2].c_str());
        // count = history_length;
        return;
    }

    HIST_ENTRY **history = history_list();

    if (history == nullptr) {
        std::cout << "No history entries" << std::endl;
        return;
    }

    int total = 0;

    while (history[total] != nullptr) {
        total++;
    }

    const int pos = args.size() == 2 ? std::max(0, std::stoi(args[1])) : 0;

    const int start = pos > 0 ? total - pos : 0;

    // Iterate through history
    for (int i = start; history[i] != nullptr; i++) {
        std::cout << "     " << i << " " << history[i]->line << std::endl;
    }
}

std::string HISTORY::get_name() {
    return this->name;
}


// JOB class Implementation

std::vector<JOB::job_info> JOB::background_jobs;



JOB::JOB() : Builtin("jobs") {
};

std::string JOB::get_name() {
    return this->name;
}

JOB &JOB::getInstance() {
    static JOB instance;
    return instance;
}


void reorder_marker(auto &background_jobs) {

   if (background_jobs.empty()) {return;}
   if (background_jobs.size() == 1) {
       auto& bj = background_jobs.back();
       bj.marker = '+';
   } else {
       auto& bj1 = background_jobs.back();
       bj1.marker = '+';
      const auto& bj2 = background_jobs.end() -2;
       bj2->marker = '-';
   }
}


void JOB::execute(const std::vector<std::string> &) {
    auto& bj = BackgroundProcess::background_jobs;

    for (auto job = bj.begin(); job != bj.end(); ) {
        auto current = *job;
        if (current.get_status() == BackgroundProcess::Status::NOT_FOUND ) {
              ++job;
        }
        else if (current.get_status() == BackgroundProcess::Status::RUNNING) {
            current.print();
            ++job;
        } else {
            current.print(false);
            BackgroundProcess::pop_job(job);
        }
    }
}



// COMPLETION IMPLEMENTATION


std::unordered_map<std::string,std::string> COMPLETE::script_list;

COMPLETE::COMPLETE() : Builtin("complete") {}
std::string COMPLETE::get_name() {
    return this->name;
}

COMPLETE &COMPLETE::getInstance() {
    static COMPLETE instance;
    return instance;
}

void COMPLETE::execute(const std::vector<std::string> &args) {

    for (auto it = args.begin() + 1; it != args.end(); ++it) {
        const auto &arg = *it;

        if (arg == "-p") {
            ++it;
            const std::string &cmd = *it;
            const auto &script_it = script_list.find(cmd);

            if (script_it != script_list.end()) {
                std::cout << "complete -C " << std::quoted(script_it->second, '\'') << " " << cmd << std::endl;
            } else {
                std::cout << "complete: " << cmd << ": " << "no completion specification" << std::endl;
            }
        }
        else if (arg == "-C") {
            ++it;
            const std::string &path = *it;
            ++it;
            const std::string &cmd = *it;
            script_list[cmd] = path;
        } else if (arg == "-r") {
            ++it;
            const std::string cmd = *it;
             script_list.erase(cmd);
        }
    }

}



// Implementation of Declare  CLASS

std::unordered_map<std::string,std::string> DECLARE::variable_list;

DECLARE::DECLARE(): Builtin("declare") {}

DECLARE &DECLARE::getInstance() {
    static DECLARE instance;
    return instance;
}
std::string DECLARE::get_name() {
    return this->name;
}

void DECLARE::execute(const std::vector<std::string> &args){
       for (auto it = args.begin() + 1; it != args.end(); ++it) {
           const auto &arg = *it;
           if (arg == "-p") {
               ++it;
               const std::string variable_name = *it;
                const auto& variable_it = variable_list.find(variable_name);
               if (variable_it != variable_list.end()) {

                   const std::string& variable_value = variable_it->second;
                   std::cout << "declare -- " << variable_name << " = " << std::quoted(variable_value,'"') << std::endl;
               } else {
                   std::cout << "declare: " << variable_name << ": not found" << std::endl;
               }

           }else {
               const std::string&  variable_exp = *it;
               const std::string& delimiter = "=";

               auto parts = variable_exp | std::views::split(delimiter);

               auto parts_it = parts.begin();

               auto key = std::string((*parts_it).begin(), (*parts_it).end());
               ++parts_it;
               const auto value = std::string((*parts_it).begin(), (*parts_it).end());

               variable_list[key] = value;

               // std::cout<< std::endl;


           }
       }
}
