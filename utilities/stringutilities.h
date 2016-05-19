#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H

#include <sstream>
#include <vector>
#include <string>

/*Quelques fonctions utilitaires pour gérer les chaines de caractéres*/

//sépare une chaine en sous-chaine selon un caractére délimiteur
std::vector<std::string> split (std::string &str, char delim);

//enlève les espaces, tabulations, retour chariot à droite d'un mot
std::string trimRight(std::string str);

//pareil mais à gauche
std::string trimLeft(std::string str);

//renvoie le premier mot d'une phrase
std::string firstWord(std::string str);

//convertis une chaine en majuscule
std::string upper(std::string str);

//pareil mais en miniscule
std::string lower(std::string str);

//convertit une valeur quelconque en une chaine de caractéres
template <typename T>
T stringTo(std::string str){
    T valor;
    std::stringstream stream(str);
    stream >> valor;
    return valor;
}
#endif // STRINGUTILITIES_H
