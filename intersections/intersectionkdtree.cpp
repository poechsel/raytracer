#include "intersectionkdtree.h"


IntersectionKdTree::IntersectionKdTree(Scene *scene, bool use_r):
    IntersectionMethod(scene), _tree(0), _use_rec(use_r)
{

}




IntersectionKdTree::~IntersectionKdTree()
{
    if (_tree)
        delete _tree;
}




void IntersectionKdTree::splitTrianglesPlane(SplitPlane &plane,
                                              std::vector<uint> &T,
                                              std::vector<uint> &tg,
                                              std::vector<uint> &td)
{
    int nd = 0, ng = 0, np = 0;
    for (uint i = 0; i < T.size(); ++i) {
        BoundingBox bb (_scene, &_scene->triangles[T[i]]);
        // si le triangle repose sur le plan séparateur on l'ajoute du côté
        // défini par le plan séparateur
        if (bb.m[plane.axis] == plane.pos && bb.M[plane.axis] == plane.pos) {
            if (plane.side == LEFT) {
                tg.push_back(T[i]);
                ng++;
            } else if (plane.side == RIGHT) {
                td.push_back(T[i]);
                nd++;
            } else {
                tg.push_back(T[i]);
                ng++;
                td.push_back(T[i]);
                nd++;
            }
        } else {    //sinon on l'ajoute à droite ou à gauche
            if (bb.m[plane.axis] < plane.pos){
                tg.push_back(T[i]);
                ng++;
            }
            if (bb.M[plane.axis] > plane.pos) {
                nd++;
                td.push_back(T[i]);
            }
        }
    }
}



KdBaseNode* IntersectionKdTree::buildTree(BoundingBox &bb,
                                            std::vector<uint> &T,
                                            uint depth)
{
    if (_offset >= 0 && getTimeElapsed(_time) > _offset) {
        this->finished = false;
        return new KdLeaf(this->_scene, {0, X, LEFT, 0}, T);
    }

    // on récupére le plan séparateur
    SplitPlane plane = this->heuristic(bb, T, depth);
    //on arréte la construction si nécessaire
    if (this->automaticEnding(plane, bb, T, depth) || !T.size()) {
        return new KdLeaf(this->_scene, plane, T);
    }
    std::vector<uint> td;
    std::vector<uint> tg;
    BoundingBox bbg;
    BoundingBox bbd;
    bb.split(plane, &bbg, &bbd);
    //on construit les fils récursivement
    this->splitTrianglesPlane(plane, T, tg, td);
    return new KdTree(_scene, plane,
                      this->buildTree(bbg, tg, depth+1),
                      this->buildTree(bbd, td, depth+1));

}




void IntersectionKdTree::build(int offset)
{
    _time = getTime();
    _offset = offset;
    BoundingBox bb;
    for (auto &triangle : this->_scene->triangles) {
        bb.expand(this->_scene, &triangle);
    }
    _bb_root = bb;
    std::vector<uint> T;
    for (int i = 0; i < this->_scene->triangles.size(); ++i) {
        T.push_back(i);
    }
    _tree = this->buildTree(bb, T, 0);
}




Real IntersectionKdTree::intersectSeq(Ray const &ray, uint *t_inter)
{
    Real t_min, t_max;
    // si on ne touche pas la racine du KdTree il n'y à pas d'intersection
    if (!intersectionBoxRay(_bb_root.getCenter(), _bb_root.getHalfSize(),
                            ray, &t_min, &t_max))
    {
        return -1;
    }
    std::stack<TempDataTraversal> stack;
    stack.push({_tree, t_min, t_max});
    while (stack.size()) {
        TempDataTraversal current = stack.top();
        KdBaseNode *cnode = current.node;
        stack.pop();
        //Tant qu'on peut aller à 'gauche' dans le parcours de l'arbre
        while (cnode->getType() != LEAF) {
            Axe a = cnode->plane.axis;
            Real t = (cnode->plane.pos - ray.origin[a]) / ray.direction[a];
            KdBaseNode *near_child = (ray.direction[a] > 0)? ((KdTree*)(cnode))->left
                                                    : ((KdTree*)(cnode))->right;
            KdBaseNode *far_child = (ray.direction[a] > 0)? ((KdTree*)(cnode))->right
                                                    : ((KdTree*)(cnode))->left;
            if (t > current.t_max) {
                cnode = near_child;
            } else if (t < current.t_min) {
                cnode = far_child;
            } else {
                // on pousse le fils 'droit' dans le stack
                stack.push({far_child, t, t_max});
                cnode = near_child;
                current.t_max = t;
            }
        }
        Real t = cnode->intersection(ray, t_inter, current.t_min, current.t_max);
        if (t >= 0)
            return t;
    }
    return -1;
}




Real IntersectionKdTree::intersect(Ray const &ray, uint *t_inter)
{
    if (this->_use_rec) {
        return this->intersectRec(ray, t_inter);
    } else {
        return this->intersectSeq(ray, t_inter);
    }
}





Real IntersectionKdTree::intersectRec(Ray const &ray, uint *t_inter)
{
    Real t_min, t_max;
    if (intersectionBoxRay(_bb_root.getCenter(), _bb_root.getHalfSize(),
                           ray, &t_min, &t_max))
    {
        return this->_tree->intersection(ray, t_inter, t_min, t_max);
    }
    return -1;
}





bool IntersectionKdTree::isFlat(SplitPlane plane, uint tri)
{
    BoundingBox clip_bb (_scene, &(_scene->triangles[tri]));
    if (plane.axis == X && clip_bb.M.x == clip_bb.m.x) {
        return true;
    } else if (plane.axis == Y && clip_bb.M.y == clip_bb.m.y) {
        return true;
    } else if (plane.axis == Z && clip_bb.M.z == clip_bb.m.z) {
        return true;
    }
    return false;
}




bool IntersectionKdTree::automaticEnding(SplitPlane &plan, BoundingBox &bb,
                                         std::vector<uint> &T, uint depth)
{
    return depth > 16 || T.size() <=  5 || bb.getVolume() == 0;
}



/******************************************************************************\
*                   Fonctions pour les KdTrees en eux mêmes                    *
\******************************************************************************/



KdBaseNode::KdBaseNode(Scene *scene, SplitPlane p):
    _scene(scene), plane(p)
{

}




KdTree::KdTree(Scene *scene, SplitPlane p, KdBaseNode* l, KdBaseNode* r):
    KdBaseNode(scene, p)
{
    this->left = l;
    this->right = r;
}




KdTree::~KdTree() {
    if(right)
        delete right;
    if(left)
        delete left;
}




Real KdTree::intersection(const Ray &ray, uint* t_inter,
                          Real t_min, Real t_max)
{
    Real d = ray.direction[this->plane.axis];
    if ( d == 0) {
        if (ray.origin[this->plane.axis] < this->plane.pos) {
            return this->left->intersection(ray, t_inter, t_min, t_max);
        }
        return this->right->intersection(ray, t_inter, t_min, t_max);
    }

    Real t = (this->plane.pos - ray.origin[this->plane.axis]) / d;

    if (t_max < t) {
        if (d > 0) {
            return this->left->intersection(ray, t_inter, t_min, t_max);
        } else {
            return this->right->intersection(ray, t_inter, t_min, t_max);
        }
    }

    if (t < t_min) {
        if (d > 0) {
            return this->right->intersection(ray, t_inter, t_min, t_max);
        } else {
            return this->left->intersection(ray, t_inter, t_min, t_max);
        }
    }

    if (d > 0) {
        Real temp_t = this->left->intersection(ray, t_inter, t_min, t);
        if (temp_t >= 0)
            return temp_t;
        return this->right->intersection(ray, t_inter, t, t_max);
    } else {
        Real temp_t = this->right->intersection(ray, t_inter, t_min, t);
        if (temp_t >= 0)
            return temp_t;
        return this->left->intersection(ray, t_inter, t, t_max);
    }

    return -1;
}




KdLeaf::KdLeaf(Scene *scene, SplitPlane p, std::vector<uint> &T):
    KdBaseNode(scene, p), _triangles(T)
{
}





Real KdLeaf::intersection(const Ray &ray, uint *t_inter, Real t_min, Real t_max)
{
    Real t = -1;
    if (this->_triangles.size())
    for (uint i = 0; i < this->_triangles.size(); ++i){
        Triangle &tri = this->_scene->triangles[this->_triangles[i]];
        Real t_temp = intersectionMoller(_scene, tri, ray);
        // un triangle n'est touché que s'il est dans cette feuille
        // ainsi t doit être inférieur à t_max
        if (t_temp >= 0 && t_temp <= t_max) {
            if (t < 0 || t_temp < t) {
                t = t_temp;
                *t_inter = this->_triangles[i];
            }
        }
    }
    return t;
}




void BoundingBox::split(SplitPlane &plane,
                        BoundingBox *left, BoundingBox *right)
{
    if (plane.axis == X) {
        left->m = this->m;
        right->M = this->M;
        left->M = Vector3f(plane.pos, this->M.y, this->M.z);
        right->m = Vector3f(plane.pos, this->m.y, this->m.z);
    } else if (plane.axis == Y) {
        left->m = this->m;
        right->M = this->M;
        left->M = Vector3f(this->M.x, plane.pos, this->M.z);
        right->m = Vector3f(this->m.x, plane.pos, this->m.z);
    } else {
        left->m = this->m;
        right->M = this->M;
        left->M = Vector3f(this->M.x, this->M.y, plane.pos);
        right->m = Vector3f(this->m.x, this->m.y, plane.pos);
    }

}
