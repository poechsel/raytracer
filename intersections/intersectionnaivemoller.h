#ifndef INTERSECTIONNAIVEMOLLER_H
#define INTERSECTIONNAIVEMOLLER_H

#include "intersectionmethod.h"
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
