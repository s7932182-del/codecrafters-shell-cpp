#include "redirection.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "parser.hpp"

Redirection::Redirection(const std::string &_file, Redirection::RTYPE rtype, Parser& ps)
{
  
   

    if(ps.is_append_mode()) {

        this->file.open(_file,std::ios::out | std::ios::app);
    }else {
         this->file.open(_file);
    }
    if (this->file.is_open())
    {

        if (rtype == RTYPE::out)
        {
            this->original = std::cout.rdbuf();
            std::cout.rdbuf(file.rdbuf());
            this->type = RTYPE::out;
        }else if(rtype == RTYPE::err) {
            this->original = std::cerr.rdbuf();
            std::cerr.rdbuf(this->file.rdbuf());
            this->type = RTYPE::err;
        }
    }
}

Redirection::~Redirection()
{

    if(type == RTYPE::out)
    std::cout.rdbuf(this->original);
    else if(type ==RTYPE::err)
    std::cerr.rdbuf(this->original);
}