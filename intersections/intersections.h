#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include "../includes.h"
#include "../geometry/Triangle.h"
#include <cfloat>

inline Real sign(Real a);

Real intersectionMoller(Scene *scene, Triangle &triangle, const Ray &ray);
Real intersectionNaive(Scene *scene, Triangle &triangle, const Ray &ray);
#endif // INTERSECTIONS_H
