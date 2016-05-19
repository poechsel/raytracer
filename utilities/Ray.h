#ifndef RAY_H
#define RAY_H

#include "../includes.h"

class Ray
{
    public:
    Ray(){}
    ~Ray(){}
    Ray (const Ray &ray) {
        origin = ray.origin;
        direction = ray.direction;
    }
    Ray (Vector3f o, Vector3f d) {
        origin = o;
        direction = d;
        origin.w = 1;
        direction.w = 0;
        direction.normalizeMe();
        inverse = (Real)1 / direction;
        inverse.normalizeMe();
    }
    //transforme un rayon selon une matrice
    void transform(Matrix4f mat) {
        origin = mat * origin;
        direction = mat * direction;
        origin /= origin.w;
        origin.w = 1.0;
        direction.w = 0.0;
        direction.normalize();
        inverse = (Real)1 / direction;
        inverse.normalizeMe();
    }
    Vector3f origin;
    Vector3f direction;
    Vector3f inverse;
};

#endif // RAY_H
