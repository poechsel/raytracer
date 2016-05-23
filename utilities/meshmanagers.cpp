#include "meshmanagers.h"

MeshManagers::MeshManagers():
    _default_mat(Color(0.8, 0.8, 0.8))
{
    //ctor
}

void MeshManagers::append(std::map<std::string, Mesh*> &others) {
    for (auto it = others.begin(); it != others.end(); ++it) {
        meshes[it->first] = (it->second);
        if (!meshes[it->first]->material) {
            meshes[it->first]->material = &_default_mat;
        }
    }
}

MeshManagers::~MeshManagers()
{
    for (auto it = meshes.begin(); it != meshes.end(); ++it)
        delete it->second;
}
