#include <cstdlib>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

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

void EchoCommand(std::vector<std::string> args){
  for(int i = 0;i < args.size();i++){
    if(i==0)continue;
    args[i].erase(std::remove(args[i].begin(), args[i].end(), ' '),args[i].end());
    std::cout << (i == 1 ? "" : " ") << args[i] << "";
  }
  std::cout << std::endl;
}

std::unordered_map<std::string, Command> commands{
  {"exit",ExitCommand},
  {"echo",EchoCommand}
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
