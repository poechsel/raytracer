#include "intersectionkdtreesah.h"

bool eventComparison(Event e1, Event e2) {
    if (e1.pos < e2.pos)
        return true;
    return e1.type < e2.type;
}


IntersectionKdTreeSAH::IntersectionKdTreeSAH(Scene *scene, Real Kt, Real Ki,
                                             bool use_r, Complexity c):
    IntersectionKdTree(scene, use_r), _Kt(Kt), _Ki(Ki), _complexity(c)
{
    //ctor
}

SplitPlane IntersectionKdTreeSAH::heuristic(BoundingBox &bb,
                                            std::vector<uint> &triangles,
                                            uint depth)
{
    if (_complexity == N2){
        return this->heuristicN2(bb, triangles, depth);
    } else if (_complexity == NLOG2N) {
        return this->heuristicNlog2n(bb, triangles, depth);
    }
}

void IntersectionKdTreeSAH::initEvents(EventsList &events,
                                        std::vector<uint> &triangles,
                                        BoundingBox &bb)
{
    events.resize(3);
    for (uint i = 0; i < triangles.size(); ++i) {
        BoundingBox clip (_scene, &(_scene->triangles[triangles[i]]));
        clip.clip(bb);
        if (clip.m[X] == clip.M[X]) {
            events[X].push_back({clip.m[X], PLANAR, triangles[i]});
        } else {
            events[X].push_back({clip.m[X], BEGIN, triangles[i]});
            events[X].push_back({clip.M[X], END, triangles[i]});
        }

        if (clip.m[Y] == clip.M[Y]) {
            events[Y].push_back({clip.m[Y], PLANAR, triangles[i]});
        } else {
            events[Y].push_back({clip.m[Y], BEGIN, triangles[i]});
            events[Y].push_back({clip.M[Y], END, triangles[i]});
        }

        if (clip.m[Z] == clip.M[Z]) {
            events[Z].push_back({clip.m[Z], PLANAR, triangles[i]});
        } else {
            events[Z].push_back({clip.m[Z], BEGIN, triangles[i]});
            events[Z].push_back({clip.M[Z], END, triangles[i]});
        }
    }
    for (uint axis = 0; axis < 3; axis++)
        std::sort(events[axis].begin(), events[axis].end());
}


SplitPlane IntersectionKdTreeSAH::getMinPlanesSorted(BoundingBox &bb,
                                                   std::vector<uint> &triangles,
                                                   uint depth,
                                                   EventsList events)
{
    SplitPlane minimal = {0.0, X, BOTH};
    Real         min_cost = -1;
    for (uint axis = 0; axis < 3; axis++) {
        int Ng = 0, Nd = triangles.size(), Np = 0;
        uint i = 0;
        Axe axe = X;
        if (axis == 1) axe = Y;
        if (axis == 2) axe = Z;
        for (uint i = 0; i < events[axis].size(); ++i) {
            int plying = 0, pending = 0, pstarting = 0;
            Real p = events[axis][i].pos;
            while (i < events[axis].size()
                   && p == events[axis][i].pos
                   && events[axis][i].type == END)
            {
                ++pending;
                i++;
            }
            while (i < events[axis].size()
                   && p == events[axis][i].pos
                   && events[axis][i].type == PLANAR)
            {
                ++plying;
                i++;
            }
            while (i < events[axis].size()
                   && p == events[axis][i].pos
                   && events[axis][i].type == BEGIN)
            {
                ++pstarting;
                i++;
            }
            Np = plying;
            Nd += -(plying + pending);
            SplitPlane plan = {p, axe, BOTH};
            TempSAH tsah = this->SAH(plan, bb, Ng, Nd, Np);
            if (0 <= tsah.cost
                && (tsah.cost < min_cost || min_cost < 0))
            {
                min_cost = tsah.cost;
                minimal = {p, axe, tsah.side, tsah.cost};
            }
            Ng += pstarting + plying;
        }
    }
    minimal.cost = min_cost;
    return minimal;
}

SplitPlane IntersectionKdTreeSAH::heuristicNlog2n(BoundingBox &bb,
                                                   std::vector<uint> &triangles,
                                                   uint depth)
{
    EventsList events;
    this->initEvents(events, triangles, bb);
    return this->getMinPlanesSorted(bb, triangles, depth, events);

}

//return a list of planes of size 2N where the 2i element is the left side of the triangle and the 2i+1 the right side
SplitPlanesList IntersectionKdTreeSAH::getPerfectSplits(BoundingBox &bb,
                                                std::vector<uint> &triangles)
{
    SplitPlanesList planes;
    planes.resize(3);
    for (auto t : triangles) {
        BoundingBox temp = getClippedAABB(_scene, t, bb);
        planes[X].push_back({temp.m.x, X, BOTH});
        planes[Y].push_back({temp.m.y, Y, BOTH});
        planes[Z].push_back({temp.m.z, Z, BOTH});
        planes[X].push_back({temp.M.x, X, BOTH});
        planes[Y].push_back({temp.M.y, Y, BOTH});
        planes[Z].push_back({temp.M.z, Z, BOTH});
    }
    return planes;
}

SplitPlane  IntersectionKdTreeSAH::heuristicN2(BoundingBox &bb,
                                               std::vector<uint> &triangles,
                                               uint depth)
{
    std::vector<SplitPlane> planes;
    for (uint i = 0; i < triangles.size(); ++i) {
        //BoundingBox temp = getClippedAABB(_scene, triangles[i], bb);
        BoundingBox temp (_scene, &(_scene->triangles[triangles[i]]));
        temp.clip(bb);
        planes.push_back({temp.m.x, X, BOTH});
        planes.push_back({temp.m.y, Y, BOTH});
        planes.push_back({temp.m.z, Z, BOTH});
        planes.push_back({temp.M.x, X, BOTH});
        planes.push_back({temp.M.y, Y, BOTH});
        planes.push_back({temp.M.z, Z, BOTH});
    }
    SplitPlane minimal = {0.0, X, BOTH};
    Real min_cost = -1;
    for (auto plan : planes) {
        int Ng = 0;
        int Nd = 0;
        int Np = 0;
        for (uint i = 0; i < triangles.size(); ++i) {
            BoundingBox bb (_scene, &_scene->triangles[triangles[i]]);
            if (bb.m[plan.axis] == plan.pos && bb.M[plan.axis] == plan.pos) {
                    Np++;
            } else {
                if (bb.m[plan.axis] < plan.pos){
                    Ng++;
                }
                if (bb.M[plan.axis] > plan.pos) {
                    Nd++;
                }
            }
        }
        TempSAH tsah = this->SAH(plan, bb, Ng, Nd, Np);
        if (0 <= tsah.cost && (tsah.cost < min_cost || min_cost < 0)) {
            min_cost = tsah.cost;
            minimal = plan;
            minimal.side = tsah.side;
            minimal.cost = tsah.cost;
        }
    }
    return minimal;
}




Real IntersectionKdTreeSAH::C(Real Pg, Real Pd, int Ng, int Nd) {
    //return (_Kt + _Ki * (Pg * (Real)Ng + Pd * (Real)Nd));
    return ((Ng == 0 || Nd == 0)? 0.8: 1.0) * (_Kt + _Ki * (Pg * Ng + Pd * Nd));
}




TempSAH IntersectionKdTreeSAH::SAH(SplitPlane &plan, BoundingBox &bb,
                                   int Ng, int Nd, int Np)
{
    BoundingBox Vg;
    BoundingBox Vd;
    bb.split(plan, &Vg, &Vd);
    if (bb.getVolume() == 0) {
        return {(Real)-1, BOTH};
    }
    Real Pg = Vg.getVolume() / bb.getVolume();
    Real Pd = Vd.getVolume() / bb.getVolume();
    Real cg = this->C(Pg, Pd, Ng + Np, Nd);
    Real cd = this->C(Pg, Pd, Ng, Nd + Np);

    if (cg < cd)
        return {cg, LEFT};
    return {cd, RIGHT};
}


IntersectionKdTreeSAH::~IntersectionKdTreeSAH()
{
}


//the automatic criterion isn't working
bool IntersectionKdTreeSAH::automaticEnding(SplitPlane &plane, BoundingBox &bb,
                                            std::vector<uint> &triangles,
                                            uint depth)
{
    //std::cout<< plane.cost <<" "<< _Ki * triangles.size()<<"\n";
    return (plane.cost == -1 || 5> triangles.size() || depth > 16 );
}//*/



//https://github.com/tomka/mitsuba-renderer/blob/master/src/libcore/triangle.cpp
int sutherlandHodgman(std::vector<Vector3f> &input, int inCount,
                      std::vector<Vector3f> &output, int axis,
                      double splitPos, bool isMinimum) {
	if (inCount < 3)
		return 0;
    //std::cout<<"test"<<input.size()<<"\n";
	Vector3f cur (input[0]);
	double sign       = isMinimum ? 1.0f : -1.0f;
	double distance   = sign * (cur[axis] - splitPos);
	bool  curIsInside = (distance >= 0);
	int   outCount    = 0;

	for (int i=0; i<inCount; ++i) {
		int nextIdx = i+1;
		if (nextIdx == inCount)
			nextIdx = 0;
		Vector3f next = input[nextIdx];
		distance = sign * (next[axis] - splitPos);
		bool nextIsInside = (distance >= 0);

		if (curIsInside && nextIsInside) {
			output.push_back(next);
			output[outCount++] = next;
		} else if (curIsInside && !nextIsInside) {
			double t = (splitPos - cur[axis]) / (next[axis] - cur[axis]);
			Vector3f p = cur + (next - cur) * t;
			p[axis] = splitPos; // Avoid roundoff errors
			output.push_back(p);
		} else if (!curIsInside && nextIsInside) {
			double t = (splitPos - cur[axis]) / (next[axis] - cur[axis]);
			Vector3f p = cur + (next - cur) * t;
			p[axis] = splitPos; // Avoid roundoff errors
			output.push_back(p);
			output.push_back(next);
		} else {
		}
		cur = next;
		curIsInside = nextIsInside;
	}
	return output.size();
}

BoundingBox getClippedAABB(const Scene *scene, const uint tri,
                           const BoundingBox &bb)
{
    std::vector<Vector3f> vertices1;
    std::vector<Vector3f> vertices2;
    vertices1.push_back(scene->vertices[scene->triangles[tri].v1]);
    vertices1.push_back(scene->vertices[scene->triangles[tri].v2]);
    vertices1.push_back(scene->vertices[scene->triangles[tri].v3]);
    uint nVertices = 3;
    for (uint axis = 0; axis < 3; ++axis) {
        nVertices = sutherlandHodgman(vertices1, nVertices, vertices2,
                                      axis, bb.m[axis], true);
        //std::cout<<vertices2.size()<<"\n";
        nVertices = sutherlandHodgman(vertices2, nVertices, vertices1,
                                      axis, bb.M[axis], false);
    }
	BoundingBox result;
	for (auto v : vertices1) {
        result.expand(v);
	}
	result.clip(bb);
	return result;
}
//*/
