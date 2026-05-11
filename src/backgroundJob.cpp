//
// Created by ani on 5/7/26.
//

#include "backgroundJob.hpp"

std::vector<BackgroundProcess::job_info> BackgroundProcess::background_jobs;
std::queue<uint32_t> BackgroundProcess::job_rank;
void reorder_marker(auto &background_jobs);

void BackgroundProcess::push_jobs(const uint32_t rank, const std::string &n, const pid_t pid) {
    if (background_jobs.empty()) {
        background_jobs.emplace_back(rank, n, pid,'+');
    } else if (background_jobs.size() == 1) {
        auto& bj1 =background_jobs.back();
        bj1.marker = '-';
        background_jobs.emplace_back(rank, n, pid,'+');
    } else {
         auto& bj1 =background_jobs.back();
        bj1.marker = '-';
        const auto bj2 = background_jobs.end() -2;
        bj2->marker = '\0';
        background_jobs.emplace_back(rank, n, pid,'+');
    }
}

void BackgroundProcess::pop_job(std::vector<job_info>::iterator& job) {
       BackgroundProcess::job_rank.push(job->rank);
       job = background_jobs.erase(job);
      if (!background_jobs.empty()) {
          reorder_marker(background_jobs);
      }

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
