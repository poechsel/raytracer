#include "intersectionnaivemoller.h"


IntersectionNaiveMoller::IntersectionNaiveMoller(Scene *scene):
    IntersectionMethod(scene){
}


void IntersectionNaiveMoller::build(int offset) {
}

Real IntersectionNaiveMoller::intersect(Ray const &ray, uint *t_inter) {
    Real t = -1;
    for (uint i = 0; i < _scene->triangles.size(); ++i){
        Real t_temp = intersectionMoller(_scene, _scene->triangles[i], ray);
        if (t_temp >= 0) {
            if (t < 0 || t_temp < t) {
                t = t_temp;
                *t_inter = i;
            }
        }
    }
    return t;
}
