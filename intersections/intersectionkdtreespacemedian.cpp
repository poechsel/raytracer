#include "intersectionkdtreespacemedian.h"

IntersectionKdTreeSpaceMedian::IntersectionKdTreeSpaceMedian(Scene *scene, bool a):
    IntersectionKdTree(scene, a)
{
    //ctor
}


SplitPlane IntersectionKdTreeSpaceMedian::heuristic(BoundingBox &bb, std::vector<uint> &T, uint depth) {
    SplitPlane plane;
    if (depth%3 == 0) {
        plane.axis = X;
        plane.pos = (bb.M.x - bb.m.x) * 0.5 + bb.m.x;
    } else if (depth%3 == 1) {
        plane.axis = Y;
        plane.pos = (bb.M.y - bb.m.y) * 0.5 + bb.m.y;
    } else {
        plane.axis = Z;
        plane.pos = (bb.M.z - bb.m.z) * 0.5 + bb.m.z;
    }
    plane.side = BOTH;
    return plane;
}

IntersectionKdTreeSpaceMedian::~IntersectionKdTreeSpaceMedian()
{
    //dtor
}
