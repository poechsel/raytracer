#include "python3dloader.h"

/*charge un environnement python puis vérifie que le plugin pour charger les
* objets est un plugin contenant les fonctions requises
*/
Python3DLoader::Python3DLoader(std::string format):
    _function(&_module)
{
    std::string upper_format = upper(format);
    std::string _module_name = "loaderOf" + upper_format;

    if(!_module.loadModule(_module_name)){
        std::cout<<"--------------------------------------------------------\n";
        std::cout<<"|  Il n'existe pas de plugins pour charger le format "
                 <<format<<"\n";
        std::cout<<"|  Rappel: il faut que le nom du loader soit: loaderOf"
                 <<upper_format<<"\n";
        std::cout<<"|_______________________________________________________\n";
    } else if(!_function.loadFunction("load")) {
        std::cout<<"--------------------------------------------------------\n";
        std::cout<<"|  Le loader doit comprendre une fonction nommée load \n";
        std::cout<<"|_______________________________________________________\n";
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

void Python3DLoader::fillFacesFromPy(Mesh *mesh, PyObject* py, uint offset){
    uint size = PyList_Size(py);
    for (uint i = 0; i < size; ++i) {
        PyObject*   tuple = PyList_GetItem(py, i);
        mesh->addFace(offset + PyLong_AsLong(PyTuple_GetItem(tuple, 0)),
                offset + PyLong_AsLong(PyTuple_GetItem(tuple, 1)),
                offset + PyLong_AsLong(PyTuple_GetItem(tuple, 2)));
    }
}
void Python3DLoader::loadSingle(Mesh *temp, PyObject *value, Scene *scene,
                                uint offset) {
    //recupere les informations de l'objet python "value". Ici seul les champs
    //vertices et triangles sont requis
    PyObject*   vertices = PyObject_GetAttrString(value, "vertices");
    PyObject*   faces = PyObject_GetAttrString(value, "triangles");
    //Créer les sommets et les faces
    this->fillVerticesFromPy(temp, vertices);
    this->fillFacesFromPy(temp, faces, offset);
    //libére la mémoire des variables pythons
    Py_DECREF(vertices);
    Py_DECREF(faces);
}

std::map<std::string,Mesh*> Python3DLoader::load(std::string path, Scene *scene) {
    PyObject*   ppath = convertToPy(path);
    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, ppath);
    PyObject* value = _function.call(args);
    Py_DECREF(args);
    std::map<std::string, Mesh*>  temps;
    //il se peut que des sommets soient déjà présents, on doit donc décaler
    //l'index des sommets suivants
    uint offset = scene->getVerticesNumber();
    std::cout<<"offset "<<offset<<"\n";
    if (PyDict_Check(value)) {
        PyObject *keys = PyDict_Keys(value);
        for (uint i = 0; i < PyList_Size(keys); ++i) {
            PyObject*   key = PyList_GetItem(keys, i);
            PyObject*   content = PyDict_GetItem(value, key);
            PyObject*   pyStr = PyUnicode_AsEncodedString(key, "utf-8", "Error ~");
            temps[std::string(PyBytes_AS_STRING(pyStr))] = new Mesh(scene);
            this->loadSingle(temps[std::string(PyBytes_AS_STRING(pyStr))],
                             content, scene, offset);
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
