#ifndef PYTHONMODULE_H
#define PYTHONMODULE_H

#include "PythonContext.h"
class PythonModule
{
    public:
        PythonModule();
        PythonModule(std::string name);
        bool loadModule(std::string name);
        virtual ~PythonModule();
        PyObject*   getPy(){return _module;}
    protected:
        PyObject*   _module;
    private:
};

#endif // PYTHONMODULE_H
