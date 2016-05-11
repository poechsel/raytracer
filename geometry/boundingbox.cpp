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
BoundingBox::BoundingBox(const Scene *scene, const Triangle *other): BoundingBox() {
    this->expand(scene, other);
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

BoundingBox::~BoundingBox()
{
    //dtor
}