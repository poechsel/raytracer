#include "boundingbox.h"

BoundingBox::BoundingBox() {
    this->m = Vector3f(std::numeric_limits<Real>::max());
    this->M = Vector3f(std::numeric_limits<Real>::lowest());
}
BoundingBox::BoundingBox(Vector3f a, Vector3f b){
    this->m = a;
    this->M = b;
}
BoundingBox::BoundingBox(const BoundingBox &other){
    this->m = other.m;
    this->M = other.M;
}
BoundingBox::BoundingBox(const Scene *scene, const Triangle *other) {
    Vector3f v1 = scene->vertices[other->v1];
    Vector3f v2 = scene->vertices[other->v2];
    Vector3f v3 = scene->vertices[other->v3];

    this->m = Vector3f (std::min(std::min(v1.x, v2.x), v3.x), std::min(std::min(v1.y, v2.y), v3.y), std::min(std::min(v1.z, v2.z), v3.z));
    this->M =Vector3f (std::max(std::max(v1.x, v2.x), v3.x), std::max(std::max(v1.y, v2.y), v3.y), std::max(std::max(v1.z, v2.z), v3.z));
}
void BoundingBox::expand(BoundingBox &other) {
    this->expand(other.m);
    this->expand(other.M);
}
void BoundingBox::expand(const Scene *scene, const Triangle *other) {
    this->expand(scene->vertices[other->v1]);
    this->expand(scene->vertices[other->v2]);
    this->expand(scene->vertices[other->v3]);
}

void BoundingBox::expand(Vector3f point) {
    this->m.x = std::min(this->m.x, point.x);
    this->m.y = std::min(this->m.y, point.y);
    this->m.z = std::min(this->m.z, point.z);
    this->M.x = std::max(this->M.x, point.x);
    this->M.y = std::max(this->M.y, point.y);
    this->M.z = std::max(this->M.z, point.z);
}

Vector3f BoundingBox::getCenter() const {
    return (Real)0.5 * (this->m + this->M);
}
Vector3f BoundingBox::getHalfSize() const {
    return (Real)0.5 * (this->M - this->m);
}
Real BoundingBox::getSurfaceArea() {
    Real l = M.x - m.x;
    Real w = M.y - m.y;
    Real h = M.z - m.z;
    return 2*l*w+2*l*h+2*w*h;


    //return (M.x - m.x) * (M.y - m.y) * (M.z - m.z);
}

void BoundingBox::clip(const BoundingBox &other) {
    this->m = Vector3f (std::max(this->m.x, other.m.x), std::max(this->m.y, other.m.y), std::max(this->m.z, other.m.z));
    this->M = Vector3f (std::min(this->M.x, other.M.x), std::min(this->M.y, other.M.y), std::min(this->M.z, other.M.z));
}

BoundingBox::~BoundingBox()
{
    //dtor
}
