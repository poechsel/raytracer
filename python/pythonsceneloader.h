#ifndef PYTHONSCENELOADER_H
#define PYTHONSCENELOADER_H

#include "../includes.h"
#include "pythonfunction.h"
#include "../utilities/camera.h"
#include "../utilities/meshmanagers.h"

class PythonSceneLoader
{
    public:
        PythonSceneLoader(std::string path);
        bool load(Camera *camera, Scene *scene);
        virtual ~PythonSceneLoader();
        bool loadObjects(PyObject *value, Scene *scene);
        bool loadScene(Camera *camera, PyObject *value);
        bool loadCamera(Camera *camera, PyObject *value);
    protected:
        std::string     _path;
        PythonModule    _module;
        PythonFunction  _function;
    private:
};

#endif // PYTHONSCENELOADER_H
