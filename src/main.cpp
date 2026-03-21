#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // * getenv() -> method
#include <sstream>
#include "executable.cpp"
#include "parser.cpp"

std::string l_trim(std::string &input)
{

  int st = 0, end = input.length() - 1;

  while (st <= end && isspace(input[st]))
    st++;
  return input.substr(st);
}

int main()
{

  while (true)
  {

    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // TODO: Uncomment the code below to pass the first stage
    std::cout << "$ ";

    // * Handle Invalid command

    std::string input;

    // std::cin >> input;
    // std::cerr << input << ": command not found" << std::endl;
    std::getline(std::cin, input);

    Parser ps(input);
     
    std::string command = ps.get_command();

    Executable executable(command);


    std::string l_trim_command = l_trim(input);

    if (command == "exit")
      break;
    else if (command == "echo")
    {
       ps.print_arg();
    }
    else if (command == "type")
    {

      std::string builtin = ps.get_argv()[1];
      std::cout << "builtin: " << builtin << std::endl;
      Executable exe(builtin);

      if (builtin == "echo" || builtin == "type" || builtin == "exit")
      {
        std::cout << builtin << " is a shell builtin" << std::endl;
      }
      else if (exe())
      {
        std::cout << builtin << " is " << exe.get_path() << std::endl;
      }
      else
      {
        std::cout << builtin << ": not found" << std::endl;
      }
    }
    
    else if(executable()) {
       executable(ps.get_argv());
    }
    
    else
    {
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
