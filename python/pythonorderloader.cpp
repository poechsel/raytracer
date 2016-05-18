#include "pythonorderloader.h"

PythonOrderLoader::PythonOrderLoader(std::string path):
    _function(&_module), _path(path)
{
    if(!_module.loadModule("loaderOfScene")){
        std::cout<<" ____________________________________________________________\n";
        std::cout<<"|  Il n'existe pas de plugins pour charger la scene \n";
        std::cout<<"|____________________________________________________________\n";
    } else {
        if(!_function.loadFunction("load")) {
            std::cout<<" ____________________________________________________________\n";
            std::cout<<"|  Le loader doit comprendre une fonction nommée load \n";
            std::cout<<"|____________________________________________________________\n";
        }
    }
}
/*
bool PythonSceneLoader::load(){
    PyObject*   ppath = convertToPy(_path);
    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, ppath);
    PyObject* value = _function.call(args);
    Py_DECREF(args);
    if (PyDict_Check(value)) {
    } else {
        return false;
    }
    Py_DECREF(value);
    return state;
}
*/
PythonOrderLoader::~PythonOrderLoader()
{
    //dtor
}
