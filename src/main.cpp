#include <iostream>
#include <string>

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

    std::string l_trim_command = l_trim(input);

    

    if (input == "exit")
      break;
    else if (l_trim_command.substr(0,5) == "echo ")
    {
      std::cout << l_trim_command.substr(5) << std::endl;
    }else if(l_trim_command.substr(0,5) == "type ") {
        std::string builtin = l_trim_command.substr(5);
        if(builtin == "echo" || builtin =="type"  || builtin == "exit") {
           std::cout << builtin <<  " is a shell builtin"  << std::endl;
        }else {
           std:: cout << builtin <<  ": not found" << std::endl; 
        }invalid_command:

    }
    else
    {
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
