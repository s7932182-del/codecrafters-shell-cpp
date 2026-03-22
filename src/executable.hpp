#ifndef EXECUTABLE_HPP
#define EXECUTABLE_HPP

#include <iostream>
#include <vector>
#include <filesystem>
#include <sstream>
#include <unistd.h>
#include<sys/wait.h>

namespace fs = std::filesystem;

class Executable
{
private:
    std::string exe;
    bool is_exe = false;
    std::string exe_path = "";

    std::vector<std::string> getEnvironmentVariable();

public:
    Executable(std::string command);
    std::string get_path();
    bool operator()();
    void operator()(const std::vector<std::string> &argv);
};

#endif
