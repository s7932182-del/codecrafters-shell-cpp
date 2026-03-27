#include "builtin.hpp"
#include<filesystem>
#include "parser.hpp"

namespace fs = std::filesystem;

// TYPE class implementation
TYPE::TYPE() : Builtin("type") {}

TYPE &TYPE::getInstance()
{
    static TYPE instance;
    return instance;
}

void TYPE::execute(Parser &ps)
{
    auto cmd_queue = ps.get_cmd_args_queue();
     
     if(cmd_queue.empty()) {
         std::cerr << "Queue is empty call form TYPE::execute" << std::endl;
     }

       std::string cmd = cmd_queue.front().argv[1];

    Executable exe(cmd);
    auto& builtinMap = Builtin::getMap();

    if  (builtinMap.count(cmd))
    {
        std::cout << cmd << " is a shell builtin" << std::endl;
    }
    else if (exe())
    {
        std::cout << cmd << " is " << exe.get_path() << std::endl;
    }
    else
    {
        std::cout << cmd << ": not found" << std::endl;
    }

    cmd_queue.pop();

    // return 1;
}

std::string TYPE::get_name()
{
    return this->name;
}

// ECHO class implementation
ECHO::ECHO() : Builtin("echo") {}

ECHO &ECHO::getInstance()
{
    static ECHO instance;
    return instance;
}

void ECHO::execute(Parser &ps)
{

    auto cmd_queue = ps.get_cmd_args_queue();
     
     if(cmd_queue.empty()) {
         std::cerr << "Queue is empty call form ECHO::execute" << std::endl;
     }

     auto argv = cmd_queue.front().argv;
     cmd_queue.pop();

     for (size_t i = 1; i < argv.size(); i++)
    {
        std::cout << argv[i] << " ";
    }
    std::cout << std::endl;

    
    // return 1;
}

std::string ECHO::get_name()
{
    return this->name;
}

// EXIT class implementation
EXIT::EXIT() : Builtin("exit") {}

EXIT &EXIT::getInstance()
{
    static EXIT instance;
    return instance;
}

void EXIT::execute(Parser& ps)
{

    ps.get_cmd_args_queue().pop();
    // return 0;
}

std::string EXIT::get_name()
{
    return this->name;
}



//  PWD class Implementation


PWD::PWD(): Builtin("pwd"){};

std::string PWD::get_name()  {
   return this->name;
}

PWD& PWD::getInstance() {
    static PWD instance;
    return instance;
}


void PWD::execute(Parser& ps) {
     ps.get_cmd_args_queue().pop();
   std::string cwd  = fs::current_path();
   std::cout << cwd << std::endl;
}



//  CD Class Implementation

CD::CD(): Builtin("cd"){};

CD& CD::getInstance(){
     static CD instance;
     return instance;
}

void CD::execute(Parser& ps) {
    // std::string dir = ps.get_argv()[1];
     auto cmd_queue = ps.get_cmd_args_queue();
     
     if(cmd_queue.empty()) {
         std::cerr << "Queue is empty call form CD::execute" << std::endl;
     }

     std::string dir = cmd_queue.front().argv[1];
     cmd_queue.pop();
    if(dir=="~")  {
       const  char * home = getenv("HOME");
       dir = home;
    }
    try {

        fs::current_path(dir);
    }catch(const fs::filesystem_error& e) {

        // std::cerr << "  What: " << e.what() << "\n";
        // std::cerr << "  Path1: " << e.path1() << "\n";

        if(e.code() == std::errc::no_such_file_or_directory) {
             std::cerr << "cd: " << dir <<  ": No such file or directory" << std::endl;
        }else if (e.code() == std::errc::permission_denied) {
            std::cerr << "  Reason: Permission denied\n";
        }
    }


}


std::string CD::get_name() {
     return this->name;
}

