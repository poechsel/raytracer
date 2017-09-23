#include "intersectionkdtreesahnlogn.h"


void merge(std::vector<Event> &a,
           std::vector<Event> &b,
           std::vector<Event> &output)
{
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



IntersectionKdTreeSAHnlogn::IntersectionKdTreeSAHnlogn(Scene *scene,
                                                       Real Kt, Real Ki,
                                                       bool use_r):
    IntersectionKdTreeSAH(scene, Kt, Ki, use_r)
{
    //ctor
}


/* on modifie la fonction de construction pour qu'on puisse passer en paramétres
* les listes d'évenements
*/
KdBaseNode* IntersectionKdTreeSAHnlogn::buildTree(EventsList &events,
                                                    BoundingBox &bb,
                                                    std::vector<uint> &T,
                                                    uint depth)
{
    if (_offset >= 0 && getTimeElapsed(_time) > _offset) {
        this->finished = false;
        return new KdLeaf(this->_scene, {0, X, LEFT, 0}, T);
    }
    SplitPlane plane = this->getMinPlanesSorted(bb, T, depth, events);
    if (this->automaticEnding(plane, bb, T, depth) || !T.size()) {
        return new KdLeaf(this->_scene, plane, T);
    }
    std::vector<uint> td;
    std::vector<uint> tg;
    BoundingBox bbg;
    BoundingBox bbd;
    bb.split(plane, &bbg, &bbd);
    this->splitTrianglesPlane(plane, T, tg, td);
    EventsList events_g;
    EventsList events_d;
    this->nextEvents(plane, bb, T, events, events_g, events_d);
    return new KdTree(_scene, plane,
                      this->buildTree(events_g, bbg, tg, depth+1),
                      this->buildTree(events_d, bbd, td, depth+1));

}



void IntersectionKdTreeSAHnlogn::build(int offset) {
    _offset = offset;
    _time = getTime();
    BoundingBox bb;
    for (auto &triangle : this->_scene->triangles) {
        bb.expand(this->_scene, &triangle);
    }
    _bb_root = bb;
    std::vector<uint> T;
    for (uint i = 0; i < this->_scene->triangles.size(); ++i) {
        T.push_back(i);
        H.push_back(BOTH);
    }
    EventsList events;
    this->initEvents(events, T, bb);
    _tree = this->buildTree(events, bb, T, 0);
}




void IntersectionKdTreeSAHnlogn::nextEvents(SplitPlane &plane,
                                             BoundingBox &bb,
                                             std::vector<uint> &T,
                                             EventsList &events,
                                             EventsList &g,
                                             EventsList &d)
{
    g.resize(3);
    d.resize(3);
    for (auto &event : events[plane.axis]) {
        H[event.triangle] = BOTH;
    }
    for (auto &event : events[plane.axis]) {
        if (event.type == PLANAR && event.pos == plane.pos)
        {
            H[event.triangle] = plane.side;
        } else if ((event.type == END || event.type == PLANAR)
                   && event.pos <= plane.pos)
        {
            H[event.triangle] = LEFT;
        } else if ((event.type == BEGIN || event.type == PLANAR)
                   && event.pos >= plane.pos)
        {
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
                temp_g.push_back({plane.pos,
                                 END,
                                 events[plane.axis][i].triangle});
            } else {
                temp_d.push_back(events[plane.axis][i]);
                temp_d.push_back({plane.pos,
                                 BEGIN,
                                 events[plane.axis][i].triangle});
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
    std::sort(temp_g.begin(), temp_g.end());
    std::sort(temp_d.begin(), temp_d.end());
    std::vector<Event> fg;
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
