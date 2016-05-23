#ifndef INTERSECTIONMETHOD_H
#define INTERSECTIONMETHOD_H
#include "../utilities/Ray.h"
#include "../geometry/Triangle.h"
#include "intersections.h"
#include "../utilities/benchmark.h"
class IntersectionMethod
{
    public:
        IntersectionMethod(Scene *scene);
        virtual ~IntersectionMethod();
        virtual void build(int offset = -1) = 0;
        /* t_inter est l'index du triangle dans notre scéne qui est touché par
        *  le rayon
        */
        virtual Real intersect(Ray const &ray, uint *t_inter) = 0;
        void setScene(Scene *scene){_scene = scene;}
        bool finished;
    protected:
        Scene *_scene;
    private:
};

#endif // INTERSECTIONMETHOD_H
