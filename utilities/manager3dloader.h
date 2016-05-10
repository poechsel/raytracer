#ifndef MANAGER3DLOADER_H
#define MANAGER3DLOADER_H

#include "../python/python3dloader.h"

class Manager3dLoader
{
    public:
        Manager3dLoader();
        std::map<std::string, Mesh*> load(std::string path, Scene *scene);
        virtual ~Manager3dLoader();
    protected:
        std::map<std::string, Python3DLoader> _loaders;
    private:
};

#endif // MANAGER3DLOADER_H
