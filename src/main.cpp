#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // * getenv() -> method
#include <sstream>
#include <unordered_map>
#include "executable.hpp"
#include "parser.hpp"
#include "builtin.hpp"
#include "redirection.hpp"
#include <readline/readline.h>
#include <readline/history.h>
#include "command_completion.hpp"
#include "command_executer.hpp"

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
  Builtin<Parser>::register_command("history", &HISTORY::getInstance());
}

int main()
{

  registerBuiltin();

  rl_attempted_completion_function = TabCompletor::my_completion;
  rl_bind_key('\t', rl_complete);

   char* histfile = getenv("HISTFILE");

   read_history(histfile);

  std::string input;
  char *line;

  while ((line = readline("$ ")) != nullptr)
  {
    // input = line;
    // free(line);

    if (strlen(line) > 0)
    {
      add_history(line);

      std::string input(line);

      // ? Parse the input - call parser constructor

      Parser ps(input);

      if(ps.get_is_exit()) {
        write_history(histfile);
        break;
      };
      if(ps.get_valid_cmd()) {
         CommandExecutor::execute(ps);
      }

    free(line);
  }


}

}
