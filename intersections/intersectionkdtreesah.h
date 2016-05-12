#ifndef INTERSECTIONKDTREESAH_H
#define INTERSECTIONKDTREESAH_H

#include "intersectionkdtree.h"
#include "../geometry/boundingbox.h"

/*
int sutherlandHodgman(std::vector<Vector3f> &input, int inCount, std::vector<Vector3f> &output, int axis,
		double splitPos, bool isMinimum);
BoundingBox getClippedAABB(const Scene *scene, const uint tri, const BoundingBox &bb);


struct TempSAH {
    Real cost;
    Side side;
};

enum EventType {
    END,
    PLANAR,
    BEGIN
};

struct Event {
    Real pos;
    EventType type;
    inline bool operator<(const Event& e) const {
			return((pos < e.pos) || (pos == e.pos && type < e.type));
	}

};

enum Complexity {
    N2,
    NLOG2N,
    NLOGN
};

bool eventComparison(Event e1, Event e2);


class IntersectionKdTreeSAH: public IntersectionKdTree
{
    public:
        IntersectionKdTreeSAH(Scene *scene, Real Kt, Real Ki, Complexity c);
        virtual ~IntersectionKdTreeSAH();
        SplitPlane  heuristic(BoundingBox &bb, std::vector<uint> &triangles, uint depth);
        SplitPlane heuristicN2(BoundingBox &bb, std::vector<uint> &triangles, uint depth);
        SplitPlane heuristicNlog2n(BoundingBox &bb, std::vector<uint> &triangles, uint depth);
        std::vector<std::vector<SplitPlane> > getPerfectSplits(BoundingBox &bb, std::vector<uint> &triangles);
    protected:
        Real _Kt;
        Real _Ki;
        Complexity _complexity;
        Real C(Real Pg, Real Pr, int Ng, int Nd);
        TempSAH SAH(SplitPlane &plan, BoundingBox &bb, int Ng, int Nd, int Np);
    private:
};
*/
#endif // INTERSECTIONKDTREESAH_H
