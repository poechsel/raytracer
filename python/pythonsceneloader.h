#ifndef PYTHONSCENELOADER_H
#define PYTHONSCENELOADER_H

#include "../includes.h"
#include "pythonfunction.h"
#include "../utilities/camera.h"
#include "../utilities/meshmanagers.h"

/* Classe qui établit un lien entre le "plugin" python permettant de charger une
*  scéne et le programme C
*/
class PythonSceneLoader
{
    public:
        PythonSceneLoader(std::string path);
        bool load(Camera *camera, Scene *scene);
        virtual ~PythonSceneLoader();
        /* charge les objects indiqués par le fichier de la scéne */
        bool loadObjects(PyObject *value, Scene *scene);
        /* Charge toute la scéne*/
        bool loadScene(Camera *camera, PyObject *value);
        /* charge la camèra*/
        bool loadCamera(Camera *camera, PyObject *value);
    protected:
        std::string     _path;
        PythonModule    _module;
        PythonFunction  _function;
    private:
};

#endif // PYTHONSCENELOADER_H
