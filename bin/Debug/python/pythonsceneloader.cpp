#include "pythonsceneloader.h"

PythonSceneLoader::PythonSceneLoader(std::string path):
    _function(&_module), _path(path)
{
    if(!_module.loadModule("loaderOfScene")){
        std::cout<<" _______________________________________________________\n";
        std::cout<<"|  Il n'existe pas de plugins pour charger la scene \n";
        std::cout<<"|_______________________________________________________\n";
    } else if(!_function.loadFunction("load")) {
        std::cout<<" _______________________________________________________\n";
        std::cout<<"|  Le loader doit comprendre une fonction nommée load \n";
        std::cout<<"|_______________________________________________________\n";
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
        //le fichier de configuration doit indiquer les paramétres essentiels au
        //moteur, comme la taille de l'image
        if (PyDict_Contains(value, key_presets)){
            PyObject *temp = PyDict_GetItem(value, key_presets);
            loadScene(camera, temp);
        } else {
            std::cout<<"No presets in the scene descriptions\n";
            state = false;
        }
        //il doit également contenir des informations concernant la caméra
        if (PyDict_Contains(value, key_camera)){
            PyObject *temp = PyDict_GetItem(value, key_camera);
            loadCamera(camera, temp);
        } else {
            std::cout<<"No Camera in the scene descriptions\n";
            state = false;
        }
        //enfin il doit y avoir au moins un objects 3D à charger
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
        PyObject *value_t = PyDict_GetItem(value, key);
        PyObject*   pyStr = PyUnicode_AsEncodedString(value_t, "utf-8", "Error ~");

        Manager3dLoader loader;
        auto meshes = loader.load(PyBytes_AS_STRING(pyStr), scene);
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

//une caméra est défini par sa matrice de transformation, son fov et son ratio
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
                camera->setMatrixCoord(PyFloat_AsDouble(PyList_GetItem(temp, j))
                                       , i, j);
            }
        }
    }
    Py_XDECREF(keymatrix);
    camera->fov = fov;
    camera->ratio = ratio;
    return true;
}

PythonSceneLoader::~PythonSceneLoader()
{
    //dtor
}
