#include "builtin.hpp"
#include <filesystem>
#include "parser.hpp"
#include <readline/history.h>
#include<iomanip>

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



JOB::JOB() : Builtin("job") {
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
    auto& bj = JOB::background_jobs;
    for (auto &job: bj) {
        // check the child process is successfully exit or not
        int state;
        const pid_t result = waitpid(job.pid, &state, WNOHANG);
        // std::string job_status = JOB::job_info::Status::RUNNING == job.status ? "Running" : "Stopped";

        if (result == -1) {
           return;
        }
        if (result == 0) {
            // std::cout << "JOB IS RUNNING" << std::endl;
            job.status = JOB::job_info::Status::RUNNING;
            std::cout << "[" << job.rank << "]" << job.marker << "  " << "Running" << std::setw(24) << job.name <<
                    std::endl;
        }
        else {
            // std::cout << "JOB IS STOPPING" << std::endl;
            if (WIFEXITED(state)) {
                // std::cout << "\n[Job " << job.pid << "] exited with status: "
                //         << WEXITSTATUS(state) << std::endl;
                job.status = JOB::job_info::Status::EXITED;



                std::cout << "[" << job.rank << "]" << job.marker << "  " << "Done" << std::setw(24)  << job.name.substr(0,job.name.size() -1) << std::endl;

            }
            else if (WIFSIGNALED(state)) {
                std::cout << "\n[Job " << job.pid << "] killed by signal: "
                        << WTERMSIG(state) << std::endl;
            }

            auto it = std::ranges::find_if(bj, [&job](const JOB::job_info &j) {
                return j.status == JOB::job_info::Status::EXITED;
            });

            if (it != bj.end()) {
                bj.erase(it);
            }
            reorder_marker(bj);

        }
    }
}
