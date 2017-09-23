#include "intersectionkdtreegeometrymedian.h"


IntersectionKdTreeGeometryMedian::IntersectionKdTreeGeometryMedian(Scene *scene,
                                                                   bool a):
    IntersectionKdTree(scene, a)
{
    //ctor
}


SplitPlane IntersectionKdTreeGeometryMedian::heuristic(BoundingBox &bb,
                                                       std::vector<uint> &T,
                                                       uint depth)
{
    //std::cout<<T.size()<<"\n";
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

    std::vector<Real> temp;
    for (uint t: T) {
        Triangle &tri = _scene->triangles[t];
        BoundingBox tri_bb (_scene, &tri);
        tri_bb.clip(bb);
        temp.push_back(tri_bb.getCenter()[plane.axis]);
    }
    std::nth_element(temp.begin(), temp.begin()+temp.size() / 2, temp.end());
    if (temp.size())
        plane.pos = temp[temp.size()/2];
    plane.side = BOTH;
    return plane;
}

IntersectionKdTreeGeometryMedian::~IntersectionKdTreeGeometryMedian()
{
    //dtor
}
