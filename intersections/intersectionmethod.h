#ifndef INTERSECTIONMETHOD_H
#define INTERSECTIONMETHOD_H
#include "../utilities/Ray.h"
#include "../geometry/Triangle.h"
#include "intersections.h"
class IntersectionMethod
{
    public:
        IntersectionMethod(Scene *scene);
        virtual ~IntersectionMethod();
        virtual void build() = 0;

        //t_inter est l'index du triangle qui s'intersecte
        virtual Real intersect(Ray const &ray, uint *t_inter) = 0;

    protected:
        Scene *_scene;
    private:
};

#endif // INTERSECTIONMETHOD_H
