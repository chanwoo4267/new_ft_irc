#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#define BLACK "\033[0;30m"
#define RED   "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE   "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN   "\033[0;36m"
#define WHITE  "\033[0;37m"
#define DEFAULT "\033[0m"

void printServerMessage(int mode, std::string message);
void printCommandMessage(int mode, int client_socket, std::string message);

std::vector<std::string> splitString(std::string str, char delim);
std::vector<std::string> splitStringWithCRLF(std::string str);

#endif