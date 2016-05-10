#include "manager3dloader.h"

Manager3dLoader::Manager3dLoader()
{
    //ctor
}

std::map<std::string, Mesh*> Manager3dLoader::load(std::string path, Scene *scene) {
    auto splits = split(path, '.');
    std::string extension = lower(splits[splits.size()-1]);
    if (_loaders.find(extension) == _loaders.end()) {
        _loaders[extension] = Python3DLoader(extension);
    }
    return _loaders[extension].load(path, scene);
}

Manager3dLoader::~Manager3dLoader()
{
    //dtor
}
