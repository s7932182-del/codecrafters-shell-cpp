#include "redirection.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "parser.hpp"

Redirection::Redirection(const std::string &_file, Redirection::RTYPE rtype)
{


    this->file.open(_file);
    if (this->file.is_open())
    {

        if (rtype == RTYPE::out)
        {
            this->original = std::cout.rdbuf();
            std::cout.rdbuf(file.rdbuf());
        }
    }
}

Redirection::~Redirection()
{
    std::cout.rdbuf(this->original);
}