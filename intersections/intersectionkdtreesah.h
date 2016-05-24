#ifndef INTERSECTIONKDTREESAH_H
#define INTERSECTIONKDTREESAH_H

#include "intersectionkdtree.h"
#include "../geometry/boundingbox.h"


int sutherlandHodgman(std::vector<Vector3f> &input, int inCount,
                      std::vector<Vector3f> &output, int axis,
                      double splitPos, bool isMinimum);
BoundingBox getClippedAABB(const Scene *scene, const uint tri,
                           const BoundingBox &bb);


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
    //this record save the index in the triangle vector which belongs to the
    //event
    uint triangle;
    inline bool operator<(const Event& e) const {
			return((pos < e.pos) || (pos == e.pos && type < e.type));
	}

};

enum Complexity {
    N2,
    NLOG2N,
    NLOGN
};

/* compare deux évenements */
bool eventComparison(Event e1, Event e2);

typedef std::vector<std::vector<SplitPlane> > SplitPlanesList;
typedef std::vector<std::vector<Event> > EventsList;


/* Implémente le SAH */
class IntersectionKdTreeSAH: public IntersectionKdTree
{
    public:
        IntersectionKdTreeSAH(Scene *scene, Real Kt, Real Ki,
                              bool use_r = true, Complexity c = NLOG2N);
        virtual ~IntersectionKdTreeSAH();

        SplitPlane          heuristic(BoundingBox &bb,
                                            std::vector<uint> &triangles,
                                            uint depth);
        /*L'heuristic implementé avec une complexité en N²*/
        SplitPlane          heuristicN2(BoundingBox &bb,
                                            std::vector<uint> &triangles,
                                            uint depth);
        /* Cette fois en nlog²n */
        SplitPlane          heuristicNlog2n(BoundingBox &bb,
                                            std::vector<uint> &triangles,
                                            uint depth);
        /* renvoit la liste des plans "parfaits" pour les triangles actuels */
        SplitPlanesList     getPerfectSplits(BoundingBox &bb,
                                            std::vector<uint> &triangles);
        /* critére d'arrêt */
        virtual bool        automaticEnding(SplitPlane &plane, BoundingBox &bb,
                                            std::vector<uint> &triangles,
                                            uint depth);
        std::vector<uint> _depth;
    protected:
        Real _Kt;
        Real _Ki;
        Complexity _complexity;

        /*Calcule le cout du plan*/
        Real C(Real Pg, Real Pr, int Ng, int Nd);
        /*Calcule le SAH pour un plan et renvoie le meilleur côté*/
        TempSAH SAH(SplitPlane &plan, BoundingBox &bb, int Ng, int Nd, int Np);
        void initEvents(EventsList &events,
                         std::vector<uint> &triangles,
                         BoundingBox &bb);
        SplitPlane       getMinPlanesSorted(BoundingBox &bb,
                                            std::vector<uint> &triangles,
                                            uint depth,
                                            EventsList events);
    private:
};

#endif // INTERSECTIONKDTREESAH_H
