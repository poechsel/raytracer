#ifndef PYTHON3DLOADER_H
#define PYTHON3DLOADER_H


#include "../includes.h"
#include "pythonfunction.h"
#include "../utilities/loadertempstructure.h"
#include "../geometry/mesh.h"
class Python3DLoader
{
    public:
        Python3DLoader(std::string format);
        Python3DLoader();
        virtual ~Python3DLoader();
        std::map<std::string, Mesh*> load(std::string path, Scene *scene);

    protected:
        std::string     _module_name;
        PythonModule    _module;
        PythonFunction  _function;
    private:
        void fillVerticesFromPy(Mesh *mesh, PyObject* py);
        void fillVerticesTexCoordsFromPy(Mesh *mesh, PyObject* py);
        void fillVerticesNormalsFromPy(Mesh *mesh, PyObject* py);
        void fillFacesFromPy(Mesh *mesh, PyObject* py, uint offset);
        void fillFacesUvFromPy(Mesh *mesh, PyObject* py);
        void fillFacesNormalsFromPy(Mesh *mesh, PyObject* py);
        void loadSingle(Mesh *mesh, PyObject *value, Scene *scene, uint offset);
};

#endif // PYTHON3DLOADER_H
