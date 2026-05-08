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
#include "backgroundJob.hpp"

std::string l_trim(std::string &input) {
  int st = 0, end = input.length() - 1;

  while (st <= end && isspace(input[st]))
    st++;
  return input.substr(st);
}

void registerBuiltin() {
  Builtin<Parser>::register_command("type", &TYPE::getInstance());
  Builtin<Parser>::register_command("echo", &ECHO::getInstance());
  Builtin<Parser>::register_command("exit", &EXIT::getInstance());
  Builtin<Parser>::register_command("pwd", &PWD::getInstance());
  Builtin<Parser>::register_command("cd", &CD::getInstance());
  Builtin<Parser>::register_command("history", &HISTORY::getInstance());
  Builtin<Parser>::register_command("jobs", &JOB::getInstance());


}

int main() {
  registerBuiltin();

  rl_attempted_completion_function = TabCompletor::my_completion;
  rl_bind_key('\t', rl_complete);

  char *histfile = getenv("HISTFILE");

  read_history(histfile);


  char *line;

  while ((line = readline("$ ")) != nullptr) {
    // input = line;
    // free(line);

    if (strlen(line) > 0) {
      add_history(line);

      std::string input(line);

      // ? Parse the input - call parser constructor

      Parser ps(input);

      const bool is_valid = ps.get_valid_cmd();

      if (ps.get_is_exit()) {
        write_history(histfile);
        break;
      };
      if (is_valid) {
        CommandExecutor::execute(ps);

       const std::string cmd = ps.get_cmd_string().substr(0, 4);

        if (cmd != "jobs") {
          auto& b_job = BackgroundProcess::background_jobs;
          for (auto job = b_job.begin(); job != b_job.end();) {
           const auto& current_job = *job;
            if (current_job.get_status() == BackgroundProcess::Status::EXITED) {
              current_job.print(false);
              BackgroundProcess::pop_job(job);
            } else  ++job;


          }
        }


      }

      free(line);
    }
  }
}
