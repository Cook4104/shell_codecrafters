#include <cstdlib>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>

std::vector<std::string> SplitString(std::string pString,char delim){
  std::vector<std::string> strings;
  std::istringstream f(pString);
  std::string cString;
  while(std::getline(f,cString,delim)){
    strings.push_back(cString);
  }
  return strings;
}


typedef void (*Command)(std::vector<std::string> args);

void ExitCommand(std::vector<std::string> args){
  exit(0);
}

void Nothing(std::vector<std::string> args){}

std::unordered_map<std::string, Command> commands{
  {"exit",ExitCommand}
};

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;  

  while(1){
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    std::vector<std::string> cmd = SplitString(input,' ');
    if(cmd.empty()) continue; 
    if(commands.find(cmd[0]) != commands.end()){
      commands[cmd[0]](cmd);
    }else{
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
