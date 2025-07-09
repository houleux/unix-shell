#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <unistd.h> 

enum shell_command {
  Echo,
  Exit,
  Type,
  Cd,
  Custom
};

std::string command_checker (std::string command); 
int command_converter (std::string command);

void echo_executer (std::stringstream &X);
void type_executer(std::stringstream &X);
void custom_executer (std::stringstream &X, const std::string command);
void cd_executer(std::stringstream &X);

std::stringstream input_taker () {
  std::string input;

  std::stringstream X(input);
  return X;
}


int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  
  while(true) {
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);

    std::stringstream X(input);
    std::string command;
    
    X >> command;

    int commandType = command_converter(command);

    switch (commandType) {
      case Echo:
        echo_executer(X);
        break;

      case Type:
        type_executer(X);
        break;

      case Custom:
        custom_executer(X, command);
        break;

      case Cd:
        cd_executer(X);
        break;

      case Exit:
        exit(0);
    }
  }

  return 0;
}

std::string command_checker (std::string command) {
  std::string env_var = getenv("PATH");
  std::stringstream T(env_var);
  
  std::string path;
  while (getline(T, path, ':')) {
    std::string abs_path = path + '/' + command;
    if(std::filesystem::exists(std::filesystem::path(abs_path))) 
      return abs_path;
  }

  return "fail";
}

int command_converter (std::string command) {
  if (command == "type") return Type;
  if (command == "exit") return Exit;
  if (command == "echo") return Echo;
  if (command == "cd")   return Cd;

  return Custom;
}

void echo_executer (std::stringstream &X) {
  std::string words;
  while (X >> words) std::cout << words << ' ';
  std::cout << std::endl;
}

void type_executer (std::stringstream &X) {
  std::string words;
  while (X >> words) {
    switch (command_converter(words)) {
      case Echo:
        std::cout << words << " is a shell builtin" << std::endl;
        break;

      case Type:
        std::cout << words << " is a shell builtin" << std::endl;
        break;

      case Exit:
        std::cout << words << " is a shell builtin" << std::endl;
        break;

      case Custom:
        std::string pp = command_checker(words);
        if (pp != "fail") 
          std::cout << words << " is " << pp << std::endl;
        else 
          std::cout << words << ": not found" << std::endl;
        break;
    }
  }
}

void cd_executer(std::stringstream &X) {
  std::string path;
  if (!(X >> path)) {
    std::cerr << "cd: missing operand" << std::endl;
    return;
  }
  
  if (chdir(path.c_str()) != 0) {
    perror("cd");
  }
}

void custom_executer (std::stringstream &X, const std::string command) {
  std::string excheck = command_checker(command);
  if (excheck != "fail") {
    std::string name, executable;
    if (X >> name)
      executable = "exec " + command + " " + name;
    else
      executable = "exec " + command;
    system(executable.c_str());
  }
  else {
    std::cout << command << ": command not found" << std::endl;
  }
} 
