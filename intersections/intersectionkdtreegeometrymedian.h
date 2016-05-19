#ifndef INTERSECTIONKDTREEGEOMETRYMEDIAN_H
#define INTERSECTIONKDTREEGEOMETRYMEDIAN_H

#include "intersectionkdtree.h"

/* KdTree utilisant l'heuristique du centre médian */
class IntersectionKdTreeGeometryMedian: public IntersectionKdTree
{
    public:
        IntersectionKdTreeGeometryMedian(Scene *scene, bool a);
        virtual ~IntersectionKdTreeGeometryMedian();
        SplitPlane  heuristic(BoundingBox &bb,
                              std::vector<uint> &triangles,
                              uint depth);
    protected:
    private:
};

#endif // INTERSECTIONKDTREEGEOMETRYMEDIAN_H
