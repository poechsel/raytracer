#ifndef INTERSECTIONKDTREE_H
#define INTERSECTIONKDTREE_H


//#include "../geometry/boundingbox.h"
#include "intersectionmethod.h"
#include <stack>
class BoundingBox;
class IntersectionMethod;
class KdBaseNode;

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

struct TempDataTraversal {
    KdBaseNode *node;
    Real t_min;
    Real t_max;
};

struct SplitPlane {
    Real pos;
    Axe axis;
    Side side;
};

enum KdTreeNodeType {
    LEAF,
    NODE
};

class IntersectionKdTree;

class KdBaseNode {
    public:
        KdBaseNode(Scene *scene);
        virtual KdTreeNodeType getType() = 0;
        virtual void build(IntersectionKdTree &ikd, BoundingBox &aabb, uint depth, std::vector<uint> T) = 0;
        virtual Real intersection(const Ray &ray, uint *t_inter, Real t_min, Real t_max) = 0;
        SplitPlane  plane;
    protected:
        Scene *_scene;

};

class KdTree: public KdBaseNode {
    public:
        KdTree(Scene *scene);
        ~KdTree();
        KdTreeNodeType getType() {return NODE;}
        void build(IntersectionKdTree &ikd, BoundingBox &aabb, uint depth, std::vector<uint> T);
        Real intersection(const Ray &ray, uint *t_inter, Real t_min, Real t_max);
        KdBaseNode      *left;
        KdBaseNode      *right;
    protected:
};

class KdLeaf: public KdBaseNode {
    public:
        KdLeaf(Scene *scene);
        KdTreeNodeType getType() {return LEAF;}
        void build(IntersectionKdTree &ikd, BoundingBox &aabb, uint depth, std::vector<uint> T);
        Real intersection(const Ray &ray, uint *t_inter, Real t_min, Real t_max);
    protected:
        std::vector<uint> _triangles;


};

class IntersectionKdTree: public IntersectionMethod{
    public:
        IntersectionKdTree(Scene *scene);
        virtual ~IntersectionKdTree();

        virtual void build();

        //t_inter est l'index du triangle qui s'intersecte
        virtual Real intersect(Ray const &ray, uint *t_inter);
        Real intersect2(Ray const &ray, uint *t_inter);
        virtual SplitPlane  heuristic(BoundingBox &bb, std::vector<uint> &triangles, uint depth) = 0;
        bool        automaticEnding(BoundingBox &bb, std::vector<uint> triangles, uint depth);
        Side        getSideTri(BoundingBox &bb, uint tri, SplitPlane plane);
        bool isFlat(SplitPlane plane, uint tri);
    protected:
        KdTree      _tree;
        BoundingBox _bb_root;
    private:
};

#endif // INTERSECTIONKDTREE_H
