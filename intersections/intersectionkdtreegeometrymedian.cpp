#include "intersectionkdtreegeometrymedian.h"


IntersectionKdTreeGeometryMedian::IntersectionKdTreeGeometryMedian(Scene *scene, bool a):
    IntersectionKdTree(scene, a)
{
    //ctor
}


SplitPlane IntersectionKdTreeGeometryMedian::heuristic(BoundingBox &bb, std::vector<uint> &T, uint depth) {
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
    Vector3f temp;
    for (uint t: T) {
        temp = temp + (Real)1/3 * (_scene->vertices[_scene->triangles[t].v1] + _scene->vertices[_scene->triangles[t].v2] + _scene->vertices[_scene->triangles[t].v3]);
    }
    plane.pos = temp[plane.axis] / T.size();
    plane.side = BOTH;
    return plane;
}

IntersectionKdTreeGeometryMedian::~IntersectionKdTreeGeometryMedian()
{
    //dtor
}
