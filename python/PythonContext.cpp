#include "PythonContext.h"


PyObject*   convertToPy(std::vector<long> vec) {
    PyObject* out = PyList_New(vec.size());
    for (unsigned int i = 0; i < vec.size(); ++i) {
        PyList_SetItem(out, i, PyLong_FromLong(vec[i]));
    }
    return out;
}

void decrefListPy(PyObject  *list){
    for (unsigned int i = 0; i <  PyList_Size(list); ++i) {
        Py_DECREF(PyList_GetItem(list, i));
    }
    Py_DECREF(list);
}
std::vector<long> convertFromPy(PyObject *py){
    std::vector<long> out(PyList_Size(py));
    for (unsigned int i = 0; i < PyList_Size(py); ++i) {
        out[i] = PyLong_AsLong(PyList_GetItem(py, i));
    }
    return out;
}



PythonContext::PythonContext()
{
    Py_Initialize();
}

void PythonContext::setCWD(std::string path) {
    PyObject* sys_path = PySys_GetObject((char*)"path");
    PyObject* program_name = convertToPy(path);
    PyList_Append(sys_path, program_name);
    Py_DECREF(program_name);
}

PythonContext::~PythonContext()
{
    Py_Finalize();
}
