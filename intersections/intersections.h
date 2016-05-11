#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include "../includes.h"
#include "../geometry/Triangle.h"
#include <cfloat>
#include "../geometry/boundingbox.h"

inline Real sign(Real a);

Real intersectionMoller(Scene *scene, Triangle &triangle, const Ray &ray);
Real intersectionNaive(Scene *scene, Triangle &triangle, const Ray &ray);
bool intersectionBoxTri(const Scene *scene, Vector3f center, Vector3f half_size, const Triangle &triangle);
bool intersectionBoxRay(Vector3f center, Vector3f half_size, const Ray &ray, Real *tmin, Real *tmax);
#endif // INTERSECTIONS_H
