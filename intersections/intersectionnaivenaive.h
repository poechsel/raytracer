#ifndef INTERSECTIONNAIVENAIVE_H
#define INTERSECTIONNAIVENAIVE_H

#include "intersectionmethod.h"

/* Test d'intersection utilisant la version naive de notre algorithme:
*    tous les triangles sont testés pour chaque rayon
*  On utilise la méthode Naive pour le calcul d'intersection entre un rayon
*  et des triangles
*/
class IntersectionNaiveNaive: public IntersectionMethod
{
    public:
        IntersectionNaiveNaive(Scene *scene);
        virtual void build();
        virtual Real intersect(Ray const &ray, uint *t_inter);
    protected:
    private:
};

#endif // INTERSECTIONNAIVENAIVE_H
