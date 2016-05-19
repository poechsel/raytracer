#ifndef MESHMANAGERS_H
#define MESHMANAGERS_H


#include "manager3dloader.h"

/* classe utilitaire permettant de g�rer facilement plusieurs mesh en interne
*  Dans notre cas �a ne nous sert pas vraiment, �a provient de la vieille
*  architecture
*/
class MeshManagers
{
    public:
        MeshManagers();
        virtual ~MeshManagers();
        //ajoute des meshs aux meshs d�j� pr�sents
        void append(std::map<std::string, Mesh*> &others);
        std::map<std::string, Mesh*> meshes;
    protected:
        Material _default_mat;
    private:
};

#endif // MESHMANAGERS_H
