#ifndef MESH_H
#define MESH_H

#include "Triangle.h"
#include "../materials/material.h"
#include "../utilities/Ray.h"
//#include "../scene.h"

class Mesh
{
    public:
        Mesh(Scene *sc);
        virtual ~Mesh();
        Material *material;
        std::vector<uint> vertices;
        std::vector<uint> triangles;
        void addVertex (Real x, Real y, Real z);
        void addVertex (Vector3f p);
        void addFace (uint v1, uint v2, uint v3, Vector3f n);
        void addFace (uint v1, uint v2, uint v3);
        void applyTransform();
        Real intersect (const Ray &ray);
        Matrix4f transform;
        Matrix4f inverse_transform;
        Matrix4f transpose;
        void setTransform(Matrix4f t);
        Vector3f normal;
        uint   hitten_triangle;
        Scene *_scene;
    protected:
        bool is_transformed;
    private:
};

#endif // MESH_H
