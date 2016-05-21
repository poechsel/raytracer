#ifndef PYTHON3DLOADER_H
#define PYTHON3DLOADER_H

#include "../includes.h"
#include "pythonfunction.h"
#include "../geometry/mesh.h"

/* Une classe utilitaire permettant de charger un plugin python qui chargera par
*  la suite un fichier 3D
*  Le fichier python qui charge l'objet doit avoir un nom de la forme "loaderOf"
*  suivi du format de fichier
*
*  Ici seul les sommets et les faces sont chargés, les coordonnées Uvs, les
*  matériaux et d'autres paramétres ne sont absolument pas gérés.
*/

class Python3DLoader
{
    public:
        Python3DLoader(std::string format);
        Python3DLoader();
        virtual ~Python3DLoader();
        /*charge le fichier contenu au chemin path dans la scéne scene*/
        std::map<std::string, Mesh*> load(std::string path, Scene *scene);
    private:
        /* charge les sommets*/
        void fillVerticesFromPy(Mesh *mesh, PyObject* py);
        /*charge les faces*/
        void fillFacesFromPy(Mesh *mesh, PyObject* py, uint offset);
        /*charge un seul mesh */
        void loadSingle(Mesh *mesh, PyObject *value, Scene *scene, uint offset);
    protected:
        std::string     _module_name;
        PythonModule    _module;
        PythonFunction  _function;
};

#endif // PYTHON3DLOADER_H
