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
    for (auto it = meshes.begin(); it !=meshes.end(); ++it) {
            std::cout<<"manager->Mesh: "<<it->first<<" -> "<<&(it->second)<<"\n";
            std::cout<<"    Faces: "<<it->second->triangles.size()
                     <<" Vertices: "<<it->second->vertices.size()<<"\n";
    }
}

MeshManagers::~MeshManagers()
{
    for (auto it = meshes.begin(); it != meshes.end(); ++it)
        delete it->second;
}
