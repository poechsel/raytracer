#include "intersectionkdtreesahnlogn.h"


void merge(std::vector<Event> &a, std::vector<Event> &b, std::vector<Event> &output){
    /*uint i_b = 0;
    for (uint i_a = 0; i_a < a.size(); ++i_a) {
        while (i_b < b.size() && b[i_b] < a[i_a]) {
            output.push_back(b[i_b]);
            i_b++;
        }
        output.push_back(a[i_a]);
    }
    while (i_b < b.size()) {
        output.push_back(b[i_b]);
        i_b++;
    }*/
    uint i_a = 0;
    uint i_b = 0;
    while (i_a < a.size() && i_b < b.size()) {
        if (a[i_a] < b[i_b]) {
            output.push_back(a[i_a]);
            i_a++;
        } else {
            output.push_back(b[i_b]);
            i_b++;
        }
    }
    while (i_a < a.size()) {
        output.push_back(a[i_a]);
        i_a++;
    }
    while (i_b < b.size()) {
        output.push_back(b[i_b]);
        i_b++;
    }
}

IntersectionKdTreeSAHnlogn::IntersectionKdTreeSAHnlogn(Scene *scene, Real Kt, Real Ki):
    IntersectionKdTreeSAH(scene, Kt, Ki, N2)
{
    //ctor
}

KdBaseNode* IntersectionKdTreeSAHnlogn::_build_tree(std::vector<std::vector<Event> > &events, BoundingBox &bb, std::vector<uint> &T, uint depth) {
    SplitPlane plane = this->heuristicNlogn(events, bb, T, depth);
    //std::cout<<plane.cost<<"\n";
    if (this->automaticEnding(plane, bb, T, depth) || !T.size()) {
        return new KdLeaf(this->_scene, plane, T);
    }
    //std::cout<<plane.cost<<"\n";
    std::vector<uint> td;
    std::vector<uint> tg;
    BoundingBox bbg;
    BoundingBox bbd;
    bb.split(plane, &bbg, &bbd);
    this->_splitTrianglesPlane(plane, T, tg, td);
    std::vector<std::vector<Event> > events_g;
    std::vector<std::vector<Event> > events_d;
    this->_nextEvents(plane, bb, T, events, events_g, events_d);
    return new KdTree(_scene, plane, this->_build_tree(events_g, bbg, tg, depth+1), this->_build_tree(events_d, bbd, td, depth+1));

}

void IntersectionKdTreeSAHnlogn::build() {
    BoundingBox bb;
    for (auto &triangle : this->_scene->triangles) {
        bb.expand(this->_scene, &triangle);
    }
    _bb_root = bb;
    std::vector<uint> T;
    for (int i = 0; i < this->_scene->triangles.size(); ++i) {
        T.push_back(i);
        H.push_back(BOTH);
    }
    std::vector<std::vector<Event> > events;
    this->_initEvents(events, T, bb);
    _tree = this->_build_tree(events, bb, T, 0);
}


SplitPlane  IntersectionKdTreeSAHnlogn::heuristicNlogn(std::vector<std::vector<Event> > &events, BoundingBox &bb, std::vector<uint> &triangles, uint depth) {
    SplitPlane minimal = {0.0, X, BOTH};
    Real         min_cost = -1;
    //auto splits =  getPerfectSplits(bb, triangles);
    //std::cout<<depth<<" : "<<events[0].size()<<" "<<events[1].size()<<" "<<events[2].size()<<"\n";
    for (uint axis = 0; axis < 3; axis++) {
        int Ng = 0, Nd = triangles.size(), Np = 0;
        uint i = 0;
        Axe axe = X;
        if (axis == 1) axe = Y;
        if (axis == 2) axe = Z;
        for (uint i = 0; i < events[axis].size(); ++i) {
            int plying = 0, pending = 0, pstarting = 0;
            Real p = events[axis][i].pos;
            while (i < events[axis].size() && p == events[axis][i].pos && events[axis][i].type == END) {
                ++pending;
                i++;
            }
            while (i < events[axis].size() && p == events[axis][i].pos && events[axis][i].type == PLANAR) {
                ++plying;
                i++;
            }
            while (i < events[axis].size() && p == events[axis][i].pos && events[axis][i].type == BEGIN) {
                ++pstarting;
                i++;
            }
            Np = plying;
            Nd += -(plying + pending);
            SplitPlane plan = {p, axe, BOTH};
            TempSAH tsah = this->SAH(plan, bb, Ng, Nd, Np);
            if (0 <= tsah.cost && (tsah.cost < min_cost || min_cost < 0)) {
                min_cost = tsah.cost;
                minimal = {p, axe, tsah.side, tsah.cost};
            }
            Ng += pstarting + plying;
        }
        //std::cout<<"vol = "<<i<<" "<<events.size()<<"\n";
        //std::cout<<bb.m<<" "<<bb.M<<"\n";
        //std::cout<<&bb<<"\n";
        //std::cout<<bb.getVolume()<<"\n";
    }
    //std::cout<<"min "<<minimal.pos<<" "<<min_cost<<"\n";
    //std::cout<<"=> "<<min_cost<<" "<<triangles.size()<<"\n";
    //std::cout<<min_cost<<"\n";
    minimal.cost = min_cost;
    return minimal;
}


std::vector<std::vector<Event> > IntersectionKdTreeSAHnlogn::_getEvents(BoundingBox &clip, uint i){
    std::vector<std::vector<Event> > events;
    events.resize(3);
    if (clip.m[X] == clip.M[X]) {
        events[X].push_back({clip.m[X], PLANAR, i});
    } else {
        events[X].push_back({clip.m[X], BEGIN, i});
        events[X].push_back({clip.M[X], END, i});
    }
    if (clip.m[Y] == clip.M[Y]) {
        events[Y].push_back({clip.m[Y], PLANAR, i});
    } else {
        events[Y].push_back({clip.m[Y], BEGIN, i});
        events[Y].push_back({clip.M[Y], END, i});
    }
    if (clip.m[Z] == clip.M[Z]) {
        events[Z].push_back({clip.m[Z], PLANAR, i});
    } else {
        events[Z].push_back({clip.m[Z], BEGIN, i});
        events[Z].push_back({clip.M[Z], END, i});
    }
    return events;
}

void IntersectionKdTreeSAHnlogn::_nextEvents(SplitPlane &plane, BoundingBox &bb, std::vector<uint> &T, std::vector<std::vector<Event> > &events, std::vector<std::vector<Event> > &g, std::vector<std::vector<Event> > &d){

    g.resize(3);
    d.resize(3);
    for (auto &event : events[plane.axis]) {
        H[event.triangle] = BOTH;
        //std::cout<<event.triangle<<" "<<H.size()<<"\n";
    }
    for (auto &event : events[plane.axis]) {
        if (event.type == PLANAR && event.pos == plane.pos) {
            H[event.triangle] = plane.side;
        } else if ((event.type == END || event.type == PLANAR) && event.pos <= plane.pos) {
            H[event.triangle] = LEFT;
        } else if ((event.type == BEGIN || event.type == PLANAR) && event.pos >= plane.pos) {
            H[event.triangle] = RIGHT;
        }
    }
    std::vector<Event> temp_g;
    std::vector<Event> temp_d;
    for (uint i = 0; i < events[plane.axis].size(); ++i) {
        if (H[events[plane.axis][i].triangle] == LEFT) {
            g[plane.axis].push_back(events[plane.axis][i]);
        } else if (H[events[plane.axis][i].triangle] == RIGHT) {
            d[plane.axis].push_back(events[plane.axis][i]);
        } else {
            if (events[plane.axis][i].type == BEGIN) {
                temp_g.push_back(events[plane.axis][i]);
                temp_g.push_back({plane.pos, END, events[plane.axis][i].triangle});
            } else {
                temp_d.push_back(events[plane.axis][i]);
                temp_d.push_back({plane.pos, BEGIN, events[plane.axis][i].triangle});
            }
        }
    }
    uint na1 = (plane.axis+1)%3;
    for (uint i = 0; i < events[na1].size(); ++i) {
        if (H[events[na1][i].triangle] == LEFT) {
            g[na1].push_back(events[na1][i]);
        } else if (H[events[na1][i].triangle] == RIGHT) {
            d[na1].push_back(events[na1][i]);
        } else {
            d[na1].push_back(events[na1][i]);
            g[na1].push_back(events[na1][i]);
        }
    }
    uint na2 = (plane.axis+2)%3;
    for (uint i = 0; i < events[na2].size(); ++i) {
        if (H[events[na2][i].triangle] == LEFT) {
            g[na2].push_back(events[na2][i]);
        } else if (H[events[na2][i].triangle] == RIGHT) {
            d[na2].push_back(events[na2][i]);
        } else {
            d[na2].push_back(events[na2][i]);
            g[na2].push_back(events[na2][i]);
        }
    }
    /*
    for (uint i = 0; i < T.size(); ++i) {
        BoundingBox clip (_scene, &(_scene->triangles[T[i]]));
        clip.clip(bb);
        std::vector<std::vector<Event> > evt_tri = this->_getEvents(clip, T[i]);
        if (H[T[i]] == LEFT) {
                //std::cout<<(plane.axis+1)%3<<" "<<(plane.axis+2)%3<<"\n";
        //std::cout<<evt_tri.size()<<"\n";
            for (auto e : evt_tri[plane.axis])
                g[plane.axis].push_back(e);
            for (auto e : evt_tri[(plane.axis+1)%3])
                g[(plane.axis+1)%3].push_back(e);
            for (auto e : evt_tri[(plane.axis+2)%3])
                g[(plane.axis+2)%3].push_back(e);
        } else if (H[T[i]] == RIGHT) {
            for (auto &e : evt_tri[plane.axis])
                d[plane.axis].push_back(e);
            for (auto &e : evt_tri[(plane.axis+1)%3])
                d[(plane.axis+1)%3].push_back(e);
            for (auto &e : evt_tri[(plane.axis+2)%3])
                d[(plane.axis+2)%3].push_back(e);
        } else {
            temp_g.push_back(evt_tri[plane.axis][0]);
            temp_g.push_back({plane.pos, END, i});
            temp_d.push_back({plane.pos, BEGIN, i});
            temp_d.push_back(evt_tri[plane.axis][1]);
            for (auto &e : evt_tri[(plane.axis+1)%3])
                g[(plane.axis+1)%3].push_back(e);
            for (auto &e : evt_tri[(plane.axis+2)%3])
                g[(plane.axis+2)%3].push_back(e);
            for (auto &e : evt_tri[(plane.axis+1)%3])
                d[(plane.axis+1)%3].push_back(e);
            for (auto &e : evt_tri[(plane.axis+2)%3])
                d[(plane.axis+2)%3].push_back(e);
        }
    }*/
    std::sort(temp_g.begin(), temp_g.end());
    std::sort(temp_d.begin(), temp_d.end());
    std::vector<Event> fg;
    //std::cout<<g[0].size()<<" "<<g[1].size()<<" "<<g[2].size()<< " <-> "<<temp_g.size()<<"\n";
    merge(g[plane.axis], temp_g, fg);
    std::vector<Event> fd;
    merge(d[plane.axis], temp_d, fd);
    g[plane.axis] = fg;
    d[plane.axis] = fd;
}

IntersectionKdTreeSAHnlogn::~IntersectionKdTreeSAHnlogn()
{
    //dtor
}
