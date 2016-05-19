#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H

#include <sstream>
#include <vector>
#include <string>

/*Quelques fonctions utilitaires pour g�rer les chaines de caract�res*/

//s�pare une chaine en sous-chaine selon un caract�re d�limiteur
std::vector<std::string> split (std::string &str, char delim);

//enl�ve les espaces, tabulations, retour chariot � droite d'un mot
std::string trimRight(std::string str);

//pareil mais � gauche
std::string trimLeft(std::string str);

//renvoie le premier mot d'une phrase
std::string firstWord(std::string str);

//convertis une chaine en majuscule
std::string upper(std::string str);

//pareil mais en miniscule
std::string lower(std::string str);

//convertit une valeur quelconque en une chaine de caract�res
template <typename T>
T stringTo(std::string str){
    T valor;
    std::stringstream stream(str);
    stream >> valor;
    return valor;
}
#endif // STRINGUTILITIES_H
