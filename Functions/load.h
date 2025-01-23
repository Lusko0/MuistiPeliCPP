#ifndef LOAD_H
#define LOAD_H

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iterator>



std::vector<std::string> Load(const std::string& filename);
std::vector<int> GetShown(std::vector<std::string> temp);
std::vector<char> GetBoard(std::vector<std::string> temp);
bool FileExists(const std::string& filename);

#endif
