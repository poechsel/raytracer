#ifndef INTERSECTIONKDTREESPACEMEDIAN_H
#define INTERSECTIONKDTREESPACEMEDIAN_H
#include "intersectionkdtree.h"

/* KdTree utilisant l'heuristique divisant l'espace selon un plan médian */
class IntersectionKdTreeSpaceMedian: public IntersectionKdTree
{
    public:
        IntersectionKdTreeSpaceMedian(Scene *scene, bool a);
        virtual ~IntersectionKdTreeSpaceMedian();
        SplitPlane  heuristic(BoundingBox &bb,
                              std::vector<uint> &triangles,
                              uint depth);
    protected:
    private:
};

#endif // INTERSECTIONKDTREESPACEMEDIAN_H
