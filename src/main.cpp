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
void PwdCommand(std::vector<std::string> args);
void CDCommand(std::vector<std::string> args);

std::unordered_map<std::string, Command> commands{
  {"exit",ExitCommand},
  {"echo",EchoCommand},
  {"type",TypeCommand},
  {"pwd",PwdCommand},
  {"cd",CDCommand}
};

std::unordered_map<std::string,std::filesystem::path> external_commands;
std::unordered_map<std::string,std::filesystem::path> local_executables;
fs::path working_directory = fs::current_path();

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

void PwdCommand(std::vector<std::string> args){
  std::cout << working_directory.string() << std::endl;
}

void CDCommand(std::vector<std::string> args){
  fs::path targetDirectory = fs::path(args[1]);

  if(args[1].find('~') != std::string::npos){
    std::string homeDir = args[1];
    size_t tildePos = homeDir.find('~');
    homeDir.replace(tildePos,1,getenv("HOME"));
    targetDirectory = fs::path(homeDir);
  }


  if(!fs::exists(targetDirectory)){
    std::cerr << "cd: " << args[1] << ": No such file or directory" << std::endl;
    return;
  }
  if(fs::is_regular_file(targetDirectory)) {
    std::cerr << "cd: not a directory: " << args[1] << std::endl;
    return;
  }
  working_directory = fs::canonical(targetDirectory);
  fs::current_path(working_directory);
}
void StartProgram(std::string command){
	system(command.c_str());
}

void GetPathCommands(){
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
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;  


  GetPathCommands();
  external_commands.clear();
  GetPathCommands();

	// Get commands in current directory
	for(const auto& entry : fs::directory_iterator(fs::current_path())){
		 if(entry.is_regular_file())
			local_executables.insert({entry.path().filename().string(),entry.path()});
	}

  while(1){
    //Update PATH
    external_commands.clear();
    GetPathCommands();

    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    std::vector<std::string> cmd = SplitString(input,' ');
		if(cmd.empty()) continue; 
   
		if(commands.find(cmd[0]) != commands.end()){
      commands[cmd[0]](cmd);
    }else{
			if(local_executables.find(cmd[0]) != local_executables.end()){
				StartProgram("./"+input);
				continue;
			}
			if(external_commands.find(cmd[0]) != external_commands.end()){
        StartProgram(input);
        continue;
			}
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
