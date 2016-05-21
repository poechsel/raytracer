#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../includes.h"
#include "../utilities/Ray.h"
#include "../materials/material.h"
#include "../scene.h"
#include "../utilities/camera.h"

class Mesh;
class Scene;
class Triangle
{
    public:
        Triangle(uint v1, uint v2, uint v3, Vector3f n, Scene *sc);
        Triangle(uint v1, uint v2, uint v3, Scene *sc);
        virtual ~Triangle();
        void recalculate();

        Scene *_scene;
        uint v1;
        uint v2;
        uint v3;
        Vector3f normal;
        Vector3f e1;
        Vector3f e2;
        Vector3f e3;
    protected:
    private:
};



class Scene
{
    public:
        Scene();
        ~Scene();
        std::vector<Vector3f> vertices;
        std::vector<Triangle> triangles;

        void appendVertices(Vector3f vert);
        uint getVerticesNumber();
        void appendTriangle(Triangle tri) ;
        uint getTrianglesNumber() ;

        void appendMeshes(std::map<std::string, Mesh*> &others);
        std::map<std::string, Mesh*>    meshes;
        Camera                          camera;
    protected:
        Material _default_mat;
};

#endif // TRIANGLE_H
