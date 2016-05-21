#include "pythonfunction.h"

PythonFunction::PythonFunction(PythonModule *module):
    _father_module(module), _function(0)
{
    //ctor
}

bool PythonFunction::loadFunction(std::string name){
    if (!_father_module)
        return false;
    _function = PyObject_GetAttrString(_father_module->getPy(), name.c_str());
    if (!_function || !PyCallable_Check(_function)) {
        if (PyErr_Occurred()){
            PyErr_Print();
        }
        std::cout<<"error\n";
        return false;
    }
    return true;
}

PyObject* PythonFunction::call(PyObject* arguments) {
    if (!_function)
        return false;
    PyObject* value = PyObject_CallObject(_function, arguments);
    if (value) {
        return value;
    }
    else {
        PyErr_Print();
        return NULL;
    }
}

PythonFunction::~PythonFunction()
{
    if (_function){
        Py_XDECREF(_function);
    }
}
