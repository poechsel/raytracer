#ifndef INTERSECTIONKDTREE_H
#define INTERSECTIONKDTREE_H


//#include "../geometry/boundingbox.h"
#include "intersectionmethod.h"
#include <stack>
class BoundingBox;
class IntersectionMethod;
class KdBaseNode;
class IntersectionKdTree;

enum Side {
    LEFT,
    RIGHT,
    BOTH
};

enum Axe{
    X,
    Y,
    Z
};

/* Données temporaires lors du parcours d'un KdTree */
struct TempDataTraversal {
    KdBaseNode *node;
    Real t_min;
    Real t_max;
};

/* Un plan séparateur */
struct SplitPlane {
    Real pos;
    Axe axis;
    Side side;
    Real cost;
};

/* Un noeud d'un KdTree est soit une feuille, soit un noeuds non vide */
enum KdTreeNodeType {
    LEAF,
    NODE
};


/* Squelette d'un noeud d'un KdTree */
class KdBaseNode {
    public:
        KdBaseNode(Scene *scene, SplitPlane p);
        ~KdBaseNode(){};
        virtual KdTreeNodeType  getType() = 0;
        virtual Real            intersection(const Ray &ray, uint *t_inter,
                                             Real t_min, Real t_max) = 0;
        SplitPlane  plane;
    protected:
        Scene *_scene;

};


/* Un noeud non-vide d'un KdTree */
class KdTree: public KdBaseNode {
    public:
        KdTree(Scene *scene, SplitPlane p, KdBaseNode* l, KdBaseNode* g);
        ~KdTree();
        KdTreeNodeType          getType() {return NODE;}
        Real                    intersection(const Ray &ray, uint *t_inter,
                                             Real t_min, Real t_max);
        KdBaseNode      *left;
        KdBaseNode      *right;
    protected:
};


/* une feuille d'un KdTree */
class KdLeaf: public KdBaseNode {
    public:
        KdLeaf(Scene *scene, SplitPlane p, std::vector<uint> &T);
        KdTreeNodeType          getType() {return LEAF;}
        Real                    intersection(const Ray &ray, uint *t_inter,
                                            Real t_min, Real t_max);
    protected:
        std::vector<uint> _triangles;


};

/* L'interface permettant de gérer un KdTree*/
class IntersectionKdTree: public IntersectionMethod{
    public:
        IntersectionKdTree(Scene *scene, bool ud = true);
        virtual ~IntersectionKdTree();

        virtual void            build();
        virtual Real            intersect(Ray const &ray, uint *t_inter);
        /* Parcours itératif du KdTree*/
        Real                    intersectSeq(Ray const &ray, uint *t_inter);
        /* Parcours récursif */
        Real                    intersectRec(Ray const &ray, uint *t_inter);
        /* L'heuristique à la base de la construction */
        virtual SplitPlane      heuristic(BoundingBox &bb,
                                        std::vector<uint> &triangles,
                                        uint depth) = 0;
        /* critére de terminaison de la construction */
        virtual bool            automaticEnding(SplitPlane &plan,
                                                BoundingBox &bb,
                                                std::vector<uint> &triangles,
                                                uint depth);
        /* vérifie si un triangle repose dans le plan séparateur */
        bool                    isFlat(SplitPlane plane, uint tri);
    protected:
        KdBaseNode*      _tree;
        BoundingBox _bb_root;
        bool _use_rec;


        virtual KdBaseNode*     buildTree(BoundingBox &bb,
                                        std::vector<uint> &triangles,
                                        uint depth);
        /* Partage les triangles d'un noeuds non vide entre le fils droit et
        *  et le fils gauche
        */
        void                    splitTrianglesPlane(SplitPlane &plane,
                                                    std::vector<uint> &T,
                                                    std::vector<uint> &tg,
                                                    std::vector<uint> &td);
    private:
};

#endif // INTERSECTIONKDTREE_H
