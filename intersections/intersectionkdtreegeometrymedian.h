#ifndef INTERSECTIONKDTREEGEOMETRYMEDIAN_H
#define INTERSECTIONKDTREEGEOMETRYMEDIAN_H

#include "intersectionkdtree.h"

class IntersectionKdTreeGeometryMedian: public IntersectionKdTree
{
    public:
        IntersectionKdTreeGeometryMedian(Scene *scene);
        virtual ~IntersectionKdTreeGeometryMedian();
        SplitPlane  heuristic(BoundingBox &bb, std::vector<uint> triangles, uint depth);
    protected:
    private:
};

#endif // INTERSECTIONKDTREEGEOMETRYMEDIAN_H
