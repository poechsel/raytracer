#ifndef MESHMANAGERS_H
#define MESHMANAGERS_H


#include "manager3dloader.h"

/* classe utilitaire permettant de gérer facilement plusieurs mesh en interne
*  Dans notre cas ça ne nous sert pas vraiment, ça provient de la vieille
*  architecture
*/
class MeshManagers
{
    public:
        MeshManagers();
        virtual ~MeshManagers();
        //ajoute des meshs aux meshs déjà présents
        void append(std::map<std::string, Mesh*> &others);
        std::map<std::string, Mesh*> meshes;
    protected:
        Material _default_mat;
    private:
};

#endif // MESHMANAGERS_H
