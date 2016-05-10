#include "python3dloader.h"

Python3DLoader::Python3DLoader(std::string format):
    _function(&_module)
{
    std::string upper_format = upper(format);
    std::string _module_name = "loaderOf" + upper_format;

    if(!_module.loadModule(_module_name)){
        std::cout<<"------------------------------------------------------------\n";
        std::cout<<"|  Il n'existe pas de plugins pour charger le format "<<format<<"\n";
        std::cout<<"|  Rappel: il faut que le nom du loader soit: loaderOf"<<upper_format<<"\n";
        std::cout<<"|____________________________________________________________\n";
    } else {
        if(!_function.loadFunction("load")) {
            std::cout<<"------------------------------------------------------------\n";
            std::cout<<"|  Le loader doit comprendre une fonction nommée load \n";
            std::cout<<"|____________________________________________________________\n";
        }
    }

}


Python3DLoader::Python3DLoader():
    _function(&_module)
{
}

void Python3DLoader::fillVerticesFromPy(Mesh *mesh, PyObject* py){
    uint size = PyList_Size(py);
    for (uint i = 0; i < size; ++i) {
        PyObject*   tuple = PyList_GetItem(py, i);
        mesh->addVertex(PyFloat_AsDouble(PyTuple_GetItem(tuple, 0)),
                        PyFloat_AsDouble(PyTuple_GetItem(tuple, 1)),
                        PyFloat_AsDouble(PyTuple_GetItem(tuple, 2)));
    }
}
//not implemented
void Python3DLoader::fillVerticesTexCoordsFromPy(Mesh *mesh, PyObject* py){
    /*uint size = PyList_Size(py);
    for (uint i = 0; i < size; ++i) {
        PyObject*   tuple = PyList_GetItem(py, i);
        mesh->addTexCoord(PyFloat_AsDouble(PyTuple_GetItem(tuple, 0)),
                        PyFloat_AsDouble(PyTuple_GetItem(tuple, 1)));
    }*/
}
void Python3DLoader::fillVerticesNormalsFromPy(Mesh *mesh, PyObject* py){
    //std::cout<<"adding vertices "<<&mesh<<"\n";
    //not implemented
    /*
    uint size = PyList_Size(py);
    for (uint i = 0; i < size; ++i) {
        PyObject*   tuple = PyList_GetItem(py, i);
        mesh->addNormalVertex(PyFloat_AsDouble(PyTuple_GetItem(tuple, 0)),
                        PyFloat_AsDouble(PyTuple_GetItem(tuple, 1)),
                        PyFloat_AsDouble(PyTuple_GetItem(tuple, 2)));
    }*/
}

void Python3DLoader::fillFacesFromPy(Mesh *mesh, PyObject* py, uint offset){
    uint size = PyList_Size(py);
    for (uint i = 0; i < size; ++i) {
        PyObject*   tuple = PyList_GetItem(py, i);
        mesh->addFace(offset + PyLong_AsLong(PyTuple_GetItem(tuple, 0)),
                offset + PyLong_AsLong(PyTuple_GetItem(tuple, 1)),
                offset + PyLong_AsLong(PyTuple_GetItem(tuple, 2)));
    }
}
//not implemented
void Python3DLoader::fillFacesUvFromPy(Mesh *mesh, PyObject* py){
    /*
    uint size = PyList_Size(py);
    for (uint i = 0; i < size; ++i) {
        PyObject*   tuple = PyList_GetItem(py, i);
        mesh->addFaceUv(PyLong_AsLong(PyTuple_GetItem(tuple, 0)),
                PyLong_AsLong(PyTuple_GetItem(tuple, 1)),
                PyLong_AsLong(PyTuple_GetItem(tuple, 2)));
    }*/
}
void Python3DLoader::fillFacesNormalsFromPy(Mesh *mesh, PyObject* py){
    //not implemented
    /*
    uint size = PyList_Size(py);
    for (uint i = 0; i < size; ++i) {
        PyObject*   tuple = PyList_GetItem(py, i);
        mesh->addFaceNormal(PyLong_AsLong(PyTuple_GetItem(tuple, 0)),
                PyLong_AsLong(PyTuple_GetItem(tuple, 1)),
                PyLong_AsLong(PyTuple_GetItem(tuple, 2)));
    }*/
}

void Python3DLoader::loadSingle(Mesh *temp, PyObject *value, Scene *scene, uint offset) {
    PyObject*   vertices = PyObject_GetAttrString(value, "vertices");
    PyObject*   faces = PyObject_GetAttrString(value, "triangles");
    PyObject*   triangles_uvs = PyObject_GetAttrString(value, "triangles_uvs");
    PyObject*   triangles_normals = PyObject_GetAttrString(value, "triangles_normals");
    PyObject*   tex_coords = PyObject_GetAttrString(value, "tex_coords");
    PyObject*   normals = PyObject_GetAttrString(value, "normals");
    this->fillVerticesFromPy(temp, vertices);
    this->fillVerticesTexCoordsFromPy(temp, tex_coords);
    this->fillVerticesNormalsFromPy(temp, normals);
    this->fillFacesFromPy(temp, faces, offset);
    this->fillFacesUvFromPy(temp, triangles_uvs);
    this->fillFacesNormalsFromPy(temp, triangles_normals);
    Py_DECREF(vertices);
    Py_DECREF(faces);
    Py_DECREF(triangles_uvs);
    Py_DECREF(triangles_normals);
    Py_DECREF(tex_coords);
    Py_DECREF(normals);
}

std::map<std::string, Mesh*> Python3DLoader::load(std::string path, Scene *scene) {
    PyObject*   ppath = convertToPy(path);
    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, ppath);
    PyObject* value = _function.call(args);
    Py_DECREF(args);
    std::map<std::string, Mesh*>  temps;
    //when loading a new obj file we must remember that their already exists some vertices
    uint offset = scene->getVerticesNumber();
    std::cout<<"offset "<<offset<<"\n";
    if (PyDict_Check(value)) {
        PyObject *keys = PyDict_Keys(value);
        uint size = PyList_Size(keys);
        for (uint i = 0; i < size; ++i) {
            PyObject*   key = PyList_GetItem(keys, i);
            PyObject*   content = PyDict_GetItem(value, key);
            PyObject*   pyStr = PyUnicode_AsEncodedString(key, "utf-8", "Error ~");
            temps[std::string(PyBytes_AS_STRING(pyStr))] = new Mesh(scene);
            this->loadSingle(temps[std::string(PyBytes_AS_STRING(pyStr))], content, scene, offset);
            Py_XDECREF(pyStr);
        }
        Py_DECREF(keys);
    } else {
        temps["default"] = new Mesh(scene);
        this->loadSingle(temps["default"], value, scene, offset);
    }
    Py_DECREF(value);
    return temps;
}


Python3DLoader::~Python3DLoader()
{
    //dtor
}
