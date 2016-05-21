#ifndef INTERSECTIONKDTREESAHNLOGN_H
#define INTERSECTIONKDTREESAHNLOGN_H

#include "intersectionkdtreesah.h"
void merge(std::vector<Event> &a,
           std::vector<Event> &b,
           std::vector<Event> &output);

/* Comme le SAH en n log(n) requiert des modifications � la construction du
*  KdTree, on le mets dans une classe s�par�e
*/
class IntersectionKdTreeSAHnlogn: public IntersectionKdTreeSAH
{
    public:
        IntersectionKdTreeSAHnlogn(Scene *scene, Real Kt, Real Ki,
                                   bool use_r = true);
        virtual ~IntersectionKdTreeSAHnlogn();
        /*on modifie la fonction de construction pour passer la liste
        * d'�venements d'un noeud � ses fils
        */
        void                build();

    protected:
        KdBaseNode*         buildTree(EventsList &events,
                                        BoundingBox &bb,
                                        std::vector<uint> &T,
                                        uint depth);
        /* � partir d'un plan s�parateur et d'une liste d'�venements, retourne
        *  les listes d'�venements de ses fils
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
