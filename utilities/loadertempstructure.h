#ifndef LOADERTEMPSTRUCTURE_H
#define LOADERTEMPSTRUCTURE_H


#include "../includes.h"


class LoaderTempStructure
{
    public:
        LoaderTempStructure();
        virtual ~LoaderTempStructure();
        std::vector<Vector3f> vertices;
        std::vector<Vector3<uint>> triangles;
    protected:
    private:
};

#endif // LOADERTEMPSTRUCTURE_H
