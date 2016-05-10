#ifndef MESHMANAGERS_H
#define MESHMANAGERS_H


#include "manager3dloader.h"
class MeshManagers
{
    public:
        MeshManagers();
        virtual ~MeshManagers();
        void append(std::map<std::string, Mesh*> &others);
        std::map<std::string, Mesh*> meshes;
    protected:
        Material _default_mat;
    private:
};

#endif // MESHMANAGERS_H
