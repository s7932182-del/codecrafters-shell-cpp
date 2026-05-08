//
// Created by ani on 5/7/26.
//

#ifndef SHELL_STARTER_CPP_BACKGROUNDJOB_HPP
#define SHELL_STARTER_CPP_BACKGROUNDJOB_HPP


#include <cstdint>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <utility>
#include <vector>
#include <sys/wait.h>
#include <queue>
class BackgroundProcess {
public:
    enum Status { RUNNING, EXITED, NOT_FOUND };
    private:
    struct job_info {
        uint32_t rank;
        std::string name;
        pid_t pid;
        char marker = '\0';



        Status status = RUNNING; // Default to RUNNING

        job_info(const uint32_t rank, std::string n, const pid_t pid, const char marker) : rank(rank), name(std::move(n)),
            pid(pid), marker(marker) {
        }

        void print(const bool isRunning = true) const {
            if (isRunning) {
                std::cout << "[" << this->rank << "]" << this->marker << "  " << "Running " << std::setw(24) << this->
                        name <<
                        std::endl;
            } else {
                std::cout << "[" << rank << "]" << marker << "  " << "Done" << std::setw(24) << name.substr(
                    0, name.size() - 1) << std::endl;
            }
        }

        [[nodiscard]] Status get_status() const {
            int state = 0;
            const int result = waitpid(pid, &state, WNOHANG);
            if (result == -1) return Status::NOT_FOUND;
            if (result == 0) return Status::RUNNING;
            return Status::EXITED;
        }
    };



public:

    BackgroundProcess() = default;
    static std::vector<job_info> background_jobs;
    static void push_jobs(uint32_t rank, const std::string &n, pid_t pid);
    static void pop_job(std::vector<job_info>::iterator& job);
    static std::queue<uint32_t> job_rank;

};


#endif //SHELL_STARTER_CPP_BACKGROUNDJOB_HPP
