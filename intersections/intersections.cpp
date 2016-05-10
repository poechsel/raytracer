#include "intersections.h"


inline Real sign(Real a){
    return (a < 0)? -1 : 1;
}


Real intersectionMoller(Scene *scene, Triangle &triangle, const Ray &ray) {
    Vector3f P = cross(ray.direction, triangle.e2);

    Real det = dot(triangle.e1, P);
    if (det < FLT_EPSILON && -FLT_EPSILON < det) {
        return -1;
    }
    Vector3f  T = ray.origin - scene->vertices[triangle.v1];
    Real invDet = 1.f/det;

    Real u = dot(T, P) * invDet;
    if (u < 0 || u > 1) {
        return -1;
    }
    Vector3f Q = cross(T, triangle.e1);
    Real v = dot(ray.direction, Q) * invDet;
    if (v < 0 || u + v > 1) {
        return -1;
    }
    Real t = dot(triangle.e2, Q) * invDet;
    if (t < 0) {
        return -1;
    }
    return t;
}

Real intersectionNaive(Scene *scene, Triangle &triangle, const Ray &ray){
    Real dn = dot(triangle.normal, ray.direction);
    if (-FLT_EPSILON < dn && dn < FLT_EPSILON) {
        return -1;
    }
    Real t = - dot(triangle.normal, ray.origin - scene->vertices[triangle.v1]) / dn;
    if (t < 0){
        return -1;
    }
    Vector3f P = ray.origin + t * ray.direction;
    Vector3f temp = P - scene->vertices[triangle.v1];
    Vector3f AP0P1P2 = cross(triangle.e1, triangle.e2);
    Real invAP0P1P2 = 1.f / AP0P1P2.length();
    Vector3f triU = cross(P - scene->vertices[triangle.v3], temp);
    Real u = triU.length() * invAP0P1P2 * sign(dot(AP0P1P2, triU));
    if (u < 0 || u > 1) {
        return -1;
    }
    Vector3f triV = cross(temp, P - scene->vertices[triangle.v2]);
    Real v = triV.length() * invAP0P1P2  * sign(dot(AP0P1P2, triV));
    if (v < 0 || u + v > 1) {
        return -1;
    }
    return t;
}
