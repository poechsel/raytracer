#include "Triangle.h"

Triangle::Triangle(uint v1, uint v2, uint v3, Vector3f n, Scene *sc)
{
    this->_scene = sc;
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    this->e1 = _scene->vertices[this->v2] - _scene->vertices[this->v1];
    this->e2 = _scene->vertices[this->v3] - _scene->vertices[this->v1];
    this->e3 = _scene->vertices[this->v3] - _scene->vertices[this->v2];
    this->normal = n.normalize();
}
Triangle::Triangle(uint v1, uint v2, uint v3, Scene *sc)
{
    this->_scene = sc;
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    this->e1 = _scene->vertices[this->v2] - _scene->vertices[this->v1];
    this->e2 = _scene->vertices[this->v3] - _scene->vertices[this->v1];
    this->e3 = _scene->vertices[this->v3] - _scene->vertices[this->v2];
    this->normal = cross(this->e1, this->e2).normalize();
}

void Triangle::recalculate() {
    this->e1 = _scene->vertices[this->v2] - _scene->vertices[this->v1];
    this->e2 = _scene->vertices[this->v3] - _scene->vertices[this->v1];
    this->e3 = _scene->vertices[this->v3] - _scene->vertices[this->v2];
    this->normal = cross(this->e1, this->e2).normalize();
}

Triangle::~Triangle()
{
    //dtor
}







//on le place là à cause de bugs de link
Scene::Scene():
    _default_mat(Color(0.8, 0.8, 0.8)){
}

void Scene::appendTriangle(Triangle tri) {
    triangles.push_back(tri);
}
uint Scene::getTrianglesNumber() {
    return triangles.size();
}

void Scene::appendVertices(Vector3f vert) {
    vertices.push_back(vert);
}
uint Scene::getVerticesNumber() {
    return vertices.size();
}




