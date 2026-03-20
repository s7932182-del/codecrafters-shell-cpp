#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage
  std::cout << "$ ";

  // Read the user Input

  std::string input;
   
  std::cin >> input;

  std::cerr << input << ": command not found" << std::endl;

  

}

