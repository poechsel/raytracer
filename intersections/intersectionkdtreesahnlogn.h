#ifndef INTERSECTIONKDTREESAHNLOGN_H
#define INTERSECTIONKDTREESAHNLOGN_H

#include "intersectionkdtreesah.h"
void merge(std::vector<Event> &a, std::vector<Event> &b, std::vector<Event> &output);
class IntersectionKdTreeSAHnlogn: public IntersectionKdTreeSAH
{
    public:
        IntersectionKdTreeSAHnlogn(Scene *scene, Real Kt, Real Ki);
        virtual ~IntersectionKdTreeSAHnlogn();
        void build();
        SplitPlane  heuristicNlogn(std::vector<std::vector<Event> > &events, BoundingBox &bb, std::vector<uint> &triangles, uint depth);
    protected:


        std::vector<Event> _getEvents(SplitPlane &plane, BoundingBox &bb, Triangle &tri);
        KdBaseNode* _build_tree(std::vector<std::vector<Event> > &events, BoundingBox &bb, std::vector<uint> &T, uint depth);
        void _nextEvents(SplitPlane &plane, BoundingBox &bb, std::vector<uint> &T, std::vector<std::vector<Event> > &events, std::vector<std::vector<Event> > &g, std::vector<std::vector<Event> > &d);
        std::vector<std::vector<Event> > _getEvents(BoundingBox &clip, uint i);
        std::vector<Side> H;
};

#endif // INTERSECTIONKDTREESAHNLOGN_H
