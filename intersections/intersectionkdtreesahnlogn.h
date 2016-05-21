#ifndef INTERSECTIONKDTREESAHNLOGN_H
#define INTERSECTIONKDTREESAHNLOGN_H

#include "intersectionkdtreesah.h"
void merge(std::vector<Event> &a,
           std::vector<Event> &b,
           std::vector<Event> &output);

/* Comme le SAH en n log(n) requiert des modifications à la construction du
*  KdTree, on le mets dans une classe séparée
*/
class IntersectionKdTreeSAHnlogn: public IntersectionKdTreeSAH
{
    public:
        IntersectionKdTreeSAHnlogn(Scene *scene, Real Kt, Real Ki,
                                   bool use_r = true);
        virtual ~IntersectionKdTreeSAHnlogn();
        /*on modifie la fonction de construction pour passer la liste
        * d'évenements d'un noeud à ses fils
        */
        void                build();

    protected:
        KdBaseNode*         buildTree(EventsList &events,
                                        BoundingBox &bb,
                                        std::vector<uint> &T,
                                        uint depth);
        /* à partir d'un plan séparateur et d'une liste d'évenements, retourne
        *  les listes d'évenements de ses fils
        */
        void                nextEvents(SplitPlane &plane,
                                        BoundingBox &bb,
                                        std::vector<uint> &T,
                                        EventsList &events,
                                        EventsList &g,
                                        EventsList &d);
        std::vector<Side> H;
};

#endif // INTERSECTIONKDTREESAHNLOGN_H
