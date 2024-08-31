#include <cstdlib>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

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

void ExitCommand(std::vector<std::string> args);
void EchoCommand(std::vector<std::string> args);
void TypeCommand(std::vector<std::string> args);

std::unordered_map<std::string, Command> commands{
  {"exit",ExitCommand},
  {"echo",EchoCommand},
  {"type",TypeCommand}
};

std::unordered_map<std::string,std::filesystem::path> external_commands;

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

void TypeCommand(std::vector<std::string> args){
  std::string path = std::string(getenv("PATH"));
  if(commands.find(args[1]) != commands.end())
    std::cout << args[1] << " is a shell builtin" << std::endl;
  else if(external_commands.find(args[1]) != external_commands.end())
		printf("%s is %s\n",args[1].c_str(),external_commands[args[1]].c_str());
	else
		printf("%s: not found\n",args[1].c_str());
}

void StartProgram(std::string command){
	system(command.c_str());
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;  

  std::string ENV_PATH = std::string(getenv("PATH"));
  std::vector<std::string> pathStrings = SplitString(ENV_PATH, ':');

  // Get commands in PATH
   for(std::string s : pathStrings){
    fs::path currentPath = fs::path(s);

    if(!fs::exists(currentPath)) continue;
    for(const auto& entry : fs::directory_iterator(fs::path(s))){
      external_commands.insert({entry.path().filename().string(),entry.path()});
    }
  } 

  while(1){
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    std::vector<std::string> cmd = SplitString(input,' ');
		if(cmd.empty()) continue; 
    
		if(commands.find(cmd[0]) != commands.end()){
      commands[cmd[0]](cmd);
    }else{

			if(external_commands.find(cmd[0]) != external_commands.end()){
				StartProgram(input);
				continue;
			}
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
