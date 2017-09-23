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
    } else {

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
    for (uint axis = 0; axis < 3; axis++) {
        if (_complexity == NLOG2N)
            std::sort(events[axis].begin(), events[axis].end());
        else
            this->bucketSort(events[axis]);
    }
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
        while (i < events[axis].size()) {
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

void IntersectionKdTreeSAH::bucketSort(std::vector<Event> &events){
    if (!events.size())
        return;
    uint n = events.size();
    std::vector<std::vector<Event>> buckets;
    buckets.resize(n);
    uint m = 0;
    uint M = 0;
    for (uint i = 0; i < events.size(); ++i) {
        if (events[i].pos < events[m].pos) {
            m = i;
        }
        if (events[i].pos > events[M].pos) {
            M = i;
        }
    }

    for (uint i = 0; i < events.size(); ++i) {
        uint index = (events[i].pos - events[m].pos) / (events[M].pos - events[m].pos) * (n-1);
        buckets[index].push_back(events[i]);
    }

    for (uint i=0; i<n; i++)
       std::sort(buckets[i].begin(), buckets[i].end());


    uint index = 0;
    for (uint i = 0; i < n; i++){
        for (uint j = 0; j < buckets[i].size(); j++) {
          events[index++] = buckets[i][j];
        }
    }
}

SplitPlane IntersectionKdTreeSAH::heuristicNlog2n(BoundingBox &bb,
                                                   std::vector<uint> &triangles,
                                                   uint depth)
{
    EventsList events;
    this->initEvents(events, triangles, bb);
    return this->getMinPlanesSorted(bb, triangles, depth, events);

}


SplitPlane  IntersectionKdTreeSAH::heuristicN2(BoundingBox &bb,
                                               std::vector<uint> &triangles,
                                               uint depth)
{
    std::vector<SplitPlane> planes;
    for (uint i = 0; i < triangles.size(); ++i) {
        BoundingBox temp (_scene, &(_scene->triangles[triangles[i]]));
        temp.clip(bb);
        planes.push_back({temp.m.x, X, BOTH, 0, triangles[i]});
        planes.push_back({temp.m.y, Y, BOTH, 0, triangles[i]});
        planes.push_back({temp.m.z, Z, BOTH, 0, triangles[i]});
        planes.push_back({temp.M.x, X, BOTH, 0, triangles[i]});
        planes.push_back({temp.M.y, Y, BOTH, 0, triangles[i]});
        planes.push_back({temp.M.z, Z, BOTH, 0, triangles[i]});
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
    return ((Ng == 0 || Nd == 0)? 0.8: 1.0) * (_Kt + _Ki * (Pg * Ng + Pd * Nd));
}




TempSAH IntersectionKdTreeSAH::SAH(SplitPlane &plan, BoundingBox &bb,
                                   int Ng, int Nd, int Np)
{
    BoundingBox Vg;
    BoundingBox Vd;
    bb.split(plan, &Vg, &Vd);
    if (bb.getSurfaceArea() == 0) {
        return {(Real)-1, BOTH};
    }
    Real Pg = Vg.getSurfaceArea() / bb.getSurfaceArea();
    Real Pd = Vd.getSurfaceArea() / bb.getSurfaceArea();
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
bool IntersectionKdTreeSAH::automaticEnding(SplitPlane &plane, BoundingBox &bb, std::vector<uint> &triangles, uint depth)
{
    return (plane.cost == -1 || plane.cost > _Ki * triangles.size() || depth > 20 || 5 > triangles.size() );
}
