#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H

#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> split (std::string &str, char delim);
std::string trimRight(std::string str);
std::string trimLeft(std::string str);
std::string firstWord(std::string str);
std::string upper(std::string str);
std::string lower(std::string str);
template <typename T>
T stringTo(std::string str){
    T valor;
    std::stringstream stream(str);
    stream >> valor;
    return valor;
}
#endif // STRINGUTILITIES_H
