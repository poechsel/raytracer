#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "../geometry/mesh.h"
#include "../includes.h"

/*
class OBJLoader
{
    public:
        OBJLoader(std::string path);
        std::map<std::string, Mesh> load();
        virtual ~OBJLoader();
        //{"#", "v", "vt", "vn", "vp", "f", "mtllib", "usemtl", "o", "g", "s",
    protected:
        std::vector<std::string> _content;
        uint _offset;
        std::map<std::string, Mesh> _output;
        Mesh *_current_mesh;
        std::string _path;
    private:
        void processObject(std::string line);
        void processVertex(std::string line);
        void processFace(std::string line);
};
//*/
#endif // OBJLOADER_H
