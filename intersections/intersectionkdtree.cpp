#include "intersectionkdtree.h"

IntersectionKdTree::IntersectionKdTree(Scene *scene, bool use_r):
    IntersectionMethod(scene), _tree(0), _use_rec(use_r)
{

}

IntersectionKdTree::~IntersectionKdTree()
{
    delete _tree;
}


Side IntersectionKdTree::getSideTri(BoundingBox &bb, uint tri, SplitPlane plane) {
    //normally, clip the tri bb to bounding box bb
    BoundingBox clip_bb (_scene, &(_scene->triangles[tri]));

    if (plane.axis == X) {
        //if (clip_bb.M.x < -2)
        //std::cout<<plane.pos<<" et "<<clip_bb.m.x<<" , "<<clip_bb.M.x<<"\n";
        if (clip_bb.M.x < plane.pos){
            return LEFT;
        }
        if (clip_bb.m.x > plane.pos) {
            return RIGHT;
        }
        return BOTH;
    } else if (plane.axis == Y) {
        if (clip_bb.M.y < plane.pos){
            return LEFT;
        }
        if (clip_bb.m.y > plane.pos) {
            return RIGHT;
        }
        return BOTH;
    } else {
        if (clip_bb.M.z < plane.pos){
            return LEFT;
        }
        if (clip_bb.m.z > plane.pos) {
            return RIGHT;
        }
        return BOTH;
    }//*/
/*
    BoundingBox bbg;
    BoundingBox bbd;
    bb.split(plane, &bbg, &bbd);
    bool col_g = intersectionBoxTri(_scene, bbg.getCenter(), bbg.getHalfSize(), _scene->triangles[tri]);
    bool col_d = intersectionBoxTri(_scene, bbd.getCenter(), bbd.getHalfSize(), _scene->triangles[tri]);
    if (col_g && col_d)
        return BOTH;
    if (col_g)
        return LEFT;
    return RIGHT;
//*/
}

void IntersectionKdTree::_splitTrianglesPlane(SplitPlane &plane, std::vector<uint> &T, std::vector<uint> &tg, std::vector<uint> &td) {
    int nd = 0, ng = 0, np = 0;
    for (uint i = 0; i < T.size(); ++i) {
        BoundingBox bb (_scene, &_scene->triangles[T[i]]);
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
        } else {
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

KdBaseNode* IntersectionKdTree::_build_tree(BoundingBox &bb, std::vector<uint> &T, uint depth) {
    SplitPlane plane = this->heuristic(bb, T, depth);
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
    return new KdTree(_scene, plane, this->_build_tree(bbg, tg, depth+1), this->_build_tree(bbd, td, depth+1));

}
void IntersectionKdTree::build() {
    BoundingBox bb;
    for (auto &triangle : this->_scene->triangles) {
        bb.expand(this->_scene, &triangle);
    }
    _bb_root = bb;
    std::vector<uint> T;
    for (int i = 0; i < this->_scene->triangles.size(); ++i) {
        T.push_back(i);
    }
    _tree = this->_build_tree(bb, T, 0);
}

Real IntersectionKdTree::intersectSeq(Ray const &ray, uint *t_inter) {
    Real t_min, t_max;
    if (!intersectionBoxRay(_bb_root.getCenter(), _bb_root.getHalfSize(), ray, &t_min, &t_max)) {
        return -1;
    }
    std::stack<TempDataTraversal> stack;
    stack.push({_tree, t_min, t_max});
    while (stack.size()) {
        TempDataTraversal current = stack.top();
        KdBaseNode *cnode = current.node;
        stack.pop();
        while (cnode->getType() != LEAF) {
            Axe a = cnode->plane.axis;
            Real t = (cnode->plane.pos - ray.origin[a]) / ray.direction[a];
            KdBaseNode *near = (ray.direction[a] > 0)? ((KdTree*)(cnode))->left : ((KdTree*)(cnode))->right;
            KdBaseNode *far = (ray.direction[a] > 0)? ((KdTree*)(cnode))->right : ((KdTree*)(cnode))->left;
            if (t > current.t_max) {
                cnode = near;
            } else if (t < current.t_min) {
                cnode = far;
            } else {
                stack.push({far, t, t_max});
                cnode = near;
                current.t_max = t;
            }
        }
        Real t = cnode->intersection(ray, t_inter, current.t_min, current.t_max);
        if (t >= 0)
            return t;
    }
    return -1;
}
Real IntersectionKdTree::intersect(Ray const &ray, uint *t_inter) {
    if (this->_use_rec) {
        return this->intersectRec(ray, t_inter);
    } else {
        return this->intersectSeq(ray, t_inter);
    }
}
Real IntersectionKdTree::intersectRec(Ray const &ray, uint *t_inter) {
    Real t_min, t_max;
    if (intersectionBoxRay(_bb_root.getCenter(), _bb_root.getHalfSize(), ray, &t_min, &t_max)) {
        return this->_tree->intersection(ray, t_inter, t_min, t_max);
    }
    return -1;
}

bool IntersectionKdTree::isFlat(SplitPlane plane, uint tri) {
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

bool IntersectionKdTree::automaticEnding(SplitPlane &plan, BoundingBox &bb, std::vector<uint> &T, uint depth) {
    return depth > 16 || T.size() <=  5 || bb.getVolume() == 0;
}






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
    delete right;
    delete left;
}

Real KdTree::intersection(const Ray &ray, uint* t_inter, Real t_min, Real t_max) {
    if ( ray.direction[this->plane.axis] == 0) {
        if (ray.origin[this->plane.axis] < this->plane.pos) {
            return this->left->intersection(ray, t_inter, t_min, t_max);
        }
        return this->right->intersection(ray, t_inter, t_min, t_max);
    }
    Real t = (this->plane.pos - ray.origin[this->plane.axis]) / ray.direction[this->plane.axis];
//std::cout<<t_min<<" "<<t_max<<" = "<<t<<"\n";
//u peut être mettre t_min, t_far
    if (t_max < t) {
        if (ray.direction[this->plane.axis] > 0) {
            return this->left->intersection(ray, t_inter, t_min, t_max);
        } else {
            return this->right->intersection(ray, t_inter, t_min, t_max);
        }
    }
    if (t < t_min) {
        if (ray.direction[this->plane.axis] > 0) {
            return this->right->intersection(ray, t_inter, t_min, t_max);
        } else {
            return this->left->intersection(ray, t_inter, t_min, t_max);
        }
    }

    if (ray.direction[this->plane.axis] > 0) {
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
Real KdLeaf::intersection(const Ray &ray, uint *t_inter, Real t_min, Real t_max) {
    Real t = -1;
    if (this->_triangles.size())
    //std::cout<<this->_triangles.size()<<"\n";
    for (uint i = 0; i < this->_triangles.size(); ++i){
            //std::cout<<i<<"\n";
        Real t_temp = intersectionMoller(_scene, this->_scene->triangles[this->_triangles[i]], ray);
        //we look for point on the interval t_min; t_max only, other can be too far away
        if (t_temp >= 0 && t_temp <= t_max) {
            if (t < 0 || t_temp < t) {
                t = t_temp;
                *t_inter = this->_triangles[i];
            }
        }
    }
    return t;
}



void BoundingBox::split(SplitPlane &plane, BoundingBox *left, BoundingBox *right) {
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
