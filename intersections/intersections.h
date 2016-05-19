#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include "../includes.h"
#include "../geometry/Triangle.h"
#include <cfloat>
#include "../geometry/boundingbox.h"

/* retourne le signe de a: -1, 0 ou 1*/
inline Real sign(Real a);

/* effectue le calcul d'intersection entre un triangle et un rayon avec
*  l'algorithme de Moller
*/
Real intersectionMoller(Scene *scene,
                        Triangle &triangle,
                        const Ray &ray);
/* effectue le calcul d'intersection entre un triangle et un rayon avec
*  l'algorithme "naif"
*/
Real intersectionNaive(Scene *scene,
                       Triangle &triangle,
                       const Ray &ray);

/* effectue le calcul d'intersection entre un triangle et une boite avec
*  l'algorithme se basant sur le SAT
*/
bool intersectionBoxTri(const Scene *scene,
                        Vector3f center, Vector3f half_size,
                        const Triangle &triangle);
/* effectue le calcul d'intersection entre une boite et un rayon.
*  t_min et t_max représente les t d'entrée et de sortie de la boite
*  respectivement
*/
bool intersectionBoxRay(Vector3f center, Vector3f half_size,
                        const Ray &ray,
                        Real *tmin, Real *tmax);
#endif // INTERSECTIONS_H
