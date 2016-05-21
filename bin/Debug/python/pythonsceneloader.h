#ifndef PYTHONSCENELOADER_H
#define PYTHONSCENELOADER_H

#include "../includes.h"
#include "pythonfunction.h"
#include "../utilities/camera.h"
#include "../utilities/meshmanagers.h"

/* Classe qui �tablit un lien entre le "plugin" python permettant de charger une
*  sc�ne et le programme C
*/
class PythonSceneLoader
{
    public:
        PythonSceneLoader(std::string path);
        bool load(Camera *camera, Scene *scene);
        virtual ~PythonSceneLoader();
        /* charge les objects indiqu�s par le fichier de la sc�ne */
        bool loadObjects(PyObject *value, Scene *scene);
        /* Charge toute la sc�ne*/
        bool loadScene(Camera *camera, PyObject *value);
        /* charge la cam�ra*/
        bool loadCamera(Camera *camera, PyObject *value);
    protected:
        std::string     _path;
        PythonModule    _module;
        PythonFunction  _function;
    private:
};

#endif // PYTHONSCENELOADER_H
