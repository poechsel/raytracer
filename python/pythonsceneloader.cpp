#include "pythonsceneloader.h"

PythonSceneLoader::PythonSceneLoader(std::string path):
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

bool PythonSceneLoader::load(Camera *camera, Scene *scene){
    PyObject*   ppath = convertToPy(_path);
    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, ppath);
    PyObject* value = _function.call(args);
    Py_DECREF(args);
    bool state = true;
    if (PyDict_Check(value)) {
        PyObject *key_presets = PyUnicode_FromString("SCENE_PRESETS");
        PyObject *key_camera = PyUnicode_FromString("CAMERA");
        PyObject *key_objects = PyUnicode_FromString("OBJECTS");
        if (PyDict_Contains(value, key_presets)){
            PyObject *temp = PyDict_GetItem(value, key_presets);
            loadScene(camera, temp);
        } else {
            std::cout<<"No presets in the scene descriptions\n";
            state = false;
        }
        if (PyDict_Contains(value, key_camera)){
            PyObject *temp = PyDict_GetItem(value, key_camera);
            loadCamera(camera, temp);
        } else {
            std::cout<<"No Camera in the scene descriptions\n";
            state = false;
        }
        if (PyDict_Contains(value, key_objects)){
            PyObject *temp = PyDict_GetItem(value, key_objects);
            loadObjects(temp, scene);
        } else {
            std::cout<<"No objects in the scene descriptions\n";
            state = false;
        }
        Py_DECREF(key_presets);
        Py_DECREF(key_camera);
        Py_DECREF(key_objects);
    } else {
        return false;
    }
    Py_DECREF(value);
    return state;
}

bool PythonSceneLoader::loadObjects(PyObject *value, Scene *scene){
    PyObject *key = PyUnicode_FromString("path");
    if (PyDict_Contains(value, key)){
        PyObject *valuet = PyDict_GetItem(value, key);
        PyObject*   pyStr = PyUnicode_AsEncodedString(valuet, "utf-8", "Error ~");

        Manager3dLoader loader;
        std::map<std::string, Mesh*> meshes = loader.load(PyBytes_AS_STRING(pyStr), scene);
        for (auto it =meshes.begin(); it !=meshes.end(); ++it) {
            std::cout<<"->Mesh: "<<it->first<<" -> "<<&(it->second)<<"\n";
            std::cout<<"    Faces: "<<it->second->triangles.size()<<" Vertices: "<<it->second->vertices.size()<<"\n";
            //std::cout<<"    uvs: "<<it->second->tex_coords.size()<<" normals: "<<it->second->normals_vertex.size()<<"\n";
        }
        scene->appendMeshes(meshes);
        std::cout<<"loading files from: "<<PyBytes_AS_STRING(pyStr)<<"\n";
    }
    Py_XDECREF(key);
    return true;
}

bool PythonSceneLoader::loadScene(Camera *camera, PyObject *value){
    int width = 0, height = 0;
    PyObject *keywidth = PyUnicode_FromString("width");
    if (PyDict_Contains(value, keywidth)){
        width = PyLong_AsLong(PyDict_GetItem(value, keywidth));
    }
    Py_XDECREF(keywidth);
    PyObject *keyheight = PyUnicode_FromString("height");
    if (PyDict_Contains(value, keyheight)){
        height = PyLong_AsLong(PyDict_GetItem(value, keyheight));
    }
    Py_XDECREF(keyheight);
    camera->width = width;
    camera->height = height;
    std::cout<<"image will be of "<<width<<"x"<<height<<"\n";
    return true;
}

bool PythonSceneLoader::loadCamera(Camera *camera, PyObject *value){
    Real ratio = 0, fov = 0;
    PyObject *keyratio = PyUnicode_FromString("ratio");
    if (PyDict_Contains(value, keyratio)){
        ratio = PyFloat_AsDouble(PyDict_GetItem(value, keyratio));
    }
    Py_XDECREF(keyratio);
    PyObject *keyfov = PyUnicode_FromString("fov");
    if (PyDict_Contains(value, keyfov)){
        fov = PyFloat_AsDouble(PyDict_GetItem(value, keyfov));
    }
    Py_XDECREF(keyfov);

    PyObject *keymatrix = PyUnicode_FromString("matrix");
    if (PyDict_Contains(value, keymatrix)){
        PyObject *matrixpy = PyDict_GetItem(value, keymatrix);
        uint size = PyList_Size(matrixpy);
        for (uint i = 0; i < size; ++i) {
            PyObject* temp = PyList_GetItem(matrixpy, i);
            uint sizew = PyList_Size(temp);
            for (uint j = 0; j < sizew; ++j) {
                camera->setMatrixCoord(PyFloat_AsDouble(PyList_GetItem(temp, j)), i, j);
                std::cout<<PyFloat_AsDouble(PyList_GetItem(temp, j))<<" ";
            }
            std::cout<<"\n";
        }
    }
    Py_XDECREF(keymatrix);
    camera->fov = fov;
    camera->ratio = ratio;

    std::cout<<"camero will be of fov "<<fov<<" ratio "<<ratio<<"\n";
    return true;
}

PythonSceneLoader::~PythonSceneLoader()
{
    //dtor
}
