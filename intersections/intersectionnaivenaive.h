#ifndef INTERSECTIONNAIVENAIVE_H
#define INTERSECTIONNAIVENAIVE_H

#include "intersectionmethod.h"

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
