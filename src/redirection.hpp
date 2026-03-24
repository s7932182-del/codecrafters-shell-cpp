
#ifndef REDIRECTION_HPP
#define REDIRECTION_HPP


#include<iostream>
#include <fstream>
#include "parser.hpp"

class Redirection {
    std::streambuf * original;
    
    std::ofstream file;
    
    public:
    enum class RTYPE{err,out};
    Redirection::RTYPE type;
      
      Redirection(const std::string& _file, RTYPE rtype, Parser& );
      ~Redirection();
      

      Redirection(const Redirection&) = delete;
      Redirection& operator=(const Redirection&) = delete;

      
      

};

#endif