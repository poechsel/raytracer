#ifndef SPHERE_H
#define SPHERE_H

#include "../includes.h"
#include "../utilities/Ray.h"
class Sphere {
    public:
        Sphere(Vector3f center, Real radius);
        virtual ~Sphere();
        Real intersect (const Ray &ray);
        Vector3f normal;
        Matrix4f transform;
        Matrix4f inverse_transform;
        Matrix4f transpose;
        void setTransform(Matrix4f t) {
            std::cout<<t<<"\n";
            transform = t;
            inverse_transform = t.inverse();
            transpose = t.inverse().transpose();
        }
    protected:
        Real _radius;
        Vector3f _center;
    private:
};

#endif // SPHERE_H
