#include "intersectionnaivenaive.h"

IntersectionNaiveNaive::IntersectionNaiveNaive(Scene *scene):
    IntersectionMethod(scene){
}


void IntersectionNaiveNaive::build(int offset) {
}

Real IntersectionNaiveNaive::intersect(Ray const &ray, uint *t_inter) {
    Real t = -1;
    for (uint i = 0; i < _scene->triangles.size(); ++i){
        Real t_temp = intersectionNaive(_scene, _scene->triangles[i], ray);
        if (t_temp >= 0) {
            if (t < 0 || t_temp < t) {
                t = t_temp;
                *t_inter = i;
            }
        }
    }
    return t;
}
