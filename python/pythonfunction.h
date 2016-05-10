#ifndef PYTHONFUNCTION_H
#define PYTHONFUNCTION_H

#include "pythonmodule.h"
class PythonFunction
{
    public:
        PythonFunction(PythonModule *module);
        bool loadFunction(std::string name);
        PyObject*   call(PyObject* arguments);
        virtual ~PythonFunction();
    protected:
        PythonModule *_father_module;
        PyObject*   _function;
    private:
};

#endif // PYTHONFUNCTION_H
