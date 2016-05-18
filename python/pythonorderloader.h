#ifndef PYTHONORDERLOADER_H
#define PYTHONORDERLOADER_H

#include "../includes.h"
#include "pythonfunction.h"

class PythonOrderLoader
{
    public:
        PythonOrderLoader(std::string path);
       // bool load();
        virtual ~PythonOrderLoader();
    protected:
        std::string     _path;
        PythonModule    _module;
        PythonFunction  _function;
    private:
};

#endif // PYTHONORDERLOADER_H
