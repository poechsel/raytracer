#include "Sphere.h"

Sphere::Sphere(Vector3f center, Real radius):
    _radius (radius), _center (center)
{
    //ctor
}

Real Sphere::intersect (const Ray &ray) {
    /*Real t0, t1; // solutions for t if the ray intersects
        // geometric solution
    Vector3f L = _center - ray.origin;
    Real tca = L.dot(ray.direction);
    if (tca < 0) return -42;
    Real d2 = L.dot(L) - tca * tca;
    if (d2 > _radius) return -42;
    Real thc = std::sqrt(_radius - d2);
    t0 = tca - thc;
    t1 = tca + thc;
    if (t0 > t1) std::swap(t0, t1);
    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead
        if (t0 < 0) return -42; // both t0 and t1 are negative
    }
    float t = t0;
    return t;*/
    Ray new_ray = ray;
    new_ray.transform(inverse_transform);
    Real a = dot(new_ray.direction, new_ray.direction);
    Real b = 2.0 * dot(new_ray.direction, new_ray.origin);
    Real c = dot(new_ray.origin, new_ray.origin) - _radius * _radius - 1.0;  // -1 because origin is a 4 dim vector and w is set at 0 (therefore dot product is one more than if it was a vec3)
    Real delta = b * b - 4 * a * c;
    Real t0, t1, t;
    if (delta < 0)
        return -1;
    else if (delta == 0){
        t = -0.5 * b / a;
    } else{
        //printf("-> %f\n", c);
        t0 = 0.5 * (-b + std::sqrt(delta)) / a;
        t1 = 0.5 * (-b - std::sqrt(delta)) / a;
        if (t1 < t0)
            std::swap(t0, t1);
        t = (t0 < 0) ? t1 : t0;
    }
    if (t < 0)
        return -1;
    normal = new_ray.origin + t * new_ray.direction;
    normal.w = 0;
    normal = transpose * normal;
    //normal.w = 0;
    normal.normalizeMe();
    //std::cout<<normal<<"\n";
    return t;
/*
    this->_normal = new_ray.getPoint(t);
    _normal.w = 0;
    _normal = glm::transpose(_invert_transform) * _normal;
    _normal.w = 0;
    _normal = glm::normalize(_normal);*/
}

Sphere::~Sphere()
{
    //dtor
}
