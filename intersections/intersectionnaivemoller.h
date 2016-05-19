#ifndef INTERSECTIONNAIVEMOLLER_H
#define INTERSECTIONNAIVEMOLLER_H

#include "intersectionmethod.h"
/* Test d'intersection utilisant la version naive de notre algorithme:
*    tous les triangles sont testés pour chaque rayon
*  On utilise la méthode de Moller pour le calcul d'intersection entre un rayon
*  et des triangles
*/
class IntersectionNaiveMoller: public IntersectionMethod
{
    public:
        IntersectionNaiveMoller(Scene *scene);
        virtual void build();
        virtual Real intersect(Ray const &ray, uint *t_inter);
    protected:
    private:
};

#endif // INTERSECTIONNAIVEMOLLER_H
