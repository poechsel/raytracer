#include "stringutilities.h"


std::string firstWord (std::string str) {
    std::string out = "";
    for (unsigned int i = 0; i < str.size(); ++i) {
        if (str[i] == ' ' || str[i] == '\t') {
            break;
        } else {
            out += str [i];
        }
    }
    return out;
}

std::string upper(std::string str) {
    std::string upper_format = "";
    for (unsigned int i = 0; i < str.size(); ++i) {
        upper_format += (char)std::toupper(str[i]);
    }
    return upper_format;
}
std::string lower(std::string str) {
    std::string upper_format = "";
    for (unsigned int i = 0; i < str.size(); ++i) {
        upper_format += (char)std::tolower(str[i]);
    }
    return upper_format;
}

std::string trimRight(std::string str) {
    std::string temp = "";
    bool c = false;
    for (unsigned int i = 0; i < str.size(); ++i) {
        if (str[i] == ' ' || str[i] == '\t') {
            if (c) {
                temp += str [i];
            }
        } else {
            c = true;
            temp += str [i];
        }
    }
    return temp;
}
std::string trimLeft(std::string str) {
    std::string temp = "";
    bool c = false;
    for (int i = str.size(); i -->0;) { // /!\ stackoverflow
        std::string cur = str.substr(i, 1);
        if (str[i] == ' ' || str[i] == '\t') {
            if (c) {
                temp.insert(0, cur);
            }
        } else {
            c = true;
            temp.insert(0, cur);
        }
    }
    return temp;
}
std::vector<std::string> split (std::string &str, char delim) {
    std::vector<std::string> temp;
    std::string current = "";
    for (unsigned int i = 0; i < str.size(); ++i) {
        if (str[i] == delim && current != "") {
            temp.push_back(current);
            current = "";
        } else {
            current += str[i];
        }
    }
    if (current != "") {
        temp.push_back(current);
    }
    return temp;
}
