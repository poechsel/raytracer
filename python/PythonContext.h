#ifndef PYTHONCONTEXT_H
#define PYTHONCONTEXT_H

#include <string>
#include <Python.h>
#include <vector>
#include <iostream>

inline PyObject* convertToPy(std::string str) {
    return PyUnicode_FromString(str.c_str());
}
inline PyObject* convertToPy(long v) {
    return PyLong_FromLong(v);
}
inline PyObject* convertToPy(double v) {
    return PyFloat_FromDouble(v);
}
PyObject*   convertToPy(std::vector<long> vec);
std::vector<long> convertFromPy(PyObject *py);
void decrefListPy(PyObject  *list);

class PythonContext
{
    public:
        PythonContext();
        void setCWD(std::string path);
        virtual ~PythonContext();
    protected:
    private:
};

#endif // PYTHONCONTEXT_H
