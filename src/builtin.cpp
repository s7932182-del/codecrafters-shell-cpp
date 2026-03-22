#include "builtin.hpp"
#include<filesystem>

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
    std::string cmd = ps.get_argv()[1];
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
    ps.print_arg();
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

void EXIT::execute(Parser& x)
{
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
   std::string cwd  = fs::current_path();
   std::cout << cwd << std::endl;
}

