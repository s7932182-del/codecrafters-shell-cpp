#include <iostream>
#include <string>


std::string l_trim(std::string &input) {

    int st = 0, end = input.length() -1;

    while(st <= end && isspace(input[st])) st++;
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
     
    std::string echo_command = l_trim_command.substr(0,4);
    
    if(input == "exit") break;
    if(echo_command == "echo") {
       std::cout << l_trim_command.substr(4) << std::endl;
    }
    std::cout << input << ": command not found" << std::endl;
  }
}
