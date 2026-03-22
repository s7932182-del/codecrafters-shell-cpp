#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // * getenv() -> method
#include <sstream>
#include <unordered_map>
#include "executable.hpp"
#include "parser.hpp"
#include "builtin.hpp"

std::string l_trim(std::string &input)
{

  int st = 0, end = input.length() - 1;

  while (st <= end && isspace(input[st]))
    st++;
  return input.substr(st);
}

void registerBuiltin()
{
  Builtin<Parser>::register_command("type", &TYPE::getInstance());
  Builtin<Parser>::register_command("echo", &ECHO::getInstance());
  Builtin<Parser>::register_command("exit", &EXIT::getInstance());
  Builtin<Parser>::register_command("pwd", &PWD::getInstance());
  Builtin<Parser>::register_command("cd", &CD::getInstance());
}

int main()
{

  registerBuiltin();

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

    auto &builtcmd = Builtin<Parser>::getMap();

    // std::string l_trim_command = l_trim(input);

    if (builtcmd.count(command))
    {
      //  if(builtcmd[command]->execute(ps) != 1) break;

      auto &cmd = builtcmd[command];
      if (cmd->get_name() == "exit")
        break;
      cmd->execute(ps);
      continue;
      //  return 0;
    }

    if (executable())
    {
      executable(ps.get_argv());
    }

    else
    {
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
