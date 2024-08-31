#include <iostream>



int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;  

  bool isAlive = true;

  while(isAlive){
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);

    std::cout << input << ": command not found" << std::endl;
  }
}
