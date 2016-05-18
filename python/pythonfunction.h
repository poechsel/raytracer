#ifndef PYTHONFUNCTION_H
#define PYTHONFUNCTION_H

#include "pythonmodule.h"
/* Permets d'accéder aux fonctions d'un module python */
class PythonFunction
{
    public:
        //module et le module qu'on veut utiliser
        PythonFunction(PythonModule *module);
        //charge une fonction dans le module module et retourne si la fonction
        //est bien présente
        bool loadFunction(std::string name);
        //appel cette fonction
        PyObject*   call(PyObject* arguments);
        virtual ~PythonFunction();
    protected:
        PythonModule *_father_module;
        PyObject*   _function;
    private:
};

#endif // PYTHONFUNCTION_H
