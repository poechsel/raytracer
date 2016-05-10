#include "pythonmodule.h"
PythonModule::PythonModule():
    _module(0)
{

}
PythonModule::PythonModule(std::string name):
    _module(0)
{
    this->loadModule(name);
}

bool PythonModule::loadModule(std::string name) {
    PyObject* pname = convertToPy(name);
    _module = PyImport_Import(pname);
    Py_DECREF(pname);
    if (!_module) {
        PyErr_Print();
        return false;
    }
    return true;
}

PythonModule::~PythonModule()
{
    if (_module)
        Py_DECREF(_module);
}
