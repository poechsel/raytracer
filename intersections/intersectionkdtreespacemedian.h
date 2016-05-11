#ifndef INTERSECTIONKDTREESPACEMEDIAN_H
#define INTERSECTIONKDTREESPACEMEDIAN_H
#include "intersectionkdtree.h"

class IntersectionKdTreeSpaceMedian: public IntersectionKdTree
{
    public:
        IntersectionKdTreeSpaceMedian(Scene *scene);
        virtual ~IntersectionKdTreeSpaceMedian();
        SplitPlane  heuristic(BoundingBox &bb, std::vector<uint> &triangles, uint depth);
    protected:
    private:
};

#endif // INTERSECTIONKDTREESPACEMEDIAN_H
