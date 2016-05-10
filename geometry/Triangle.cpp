#include "Triangle.h"

Triangle::Triangle(uint v1, uint v2, uint v3, Vector3f n, Scene *sc)
{
    this->_scene = sc;
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    this->normal = n.normalize();
}
Triangle::Triangle(uint v1, uint v2, uint v3, Scene *sc)
{
    this->_scene = sc;
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    //std::cout<<"->"<<v1<<" "<<v2<<" "<<v3<<" "<<parent<<"\n";
    Vector3f V = _scene->vertices[v2] - _scene->vertices[v1];
    Vector3f W = _scene->vertices[v3] - _scene->vertices[v1];
    this->normal = cross(V, W).normalize();
    //std::cout<<parent->vertices[v1]<<" | "<<vset.vertices[v2]<<" | "<<vset.vertices[v3]<<"("<<v1<<" "<<v2<<" "<<v3<<")\n";
}

void Triangle::recalculate() {
    Vector3f V = _scene->vertices[v2] - _scene->vertices[v1];
    Vector3f W = _scene->vertices[v3] - _scene->vertices[v1];
    this->normal = cross(V, W).normalize();
}

Real Triangle::intersect(const Ray &ray) {/*
    std::cout<<vset.vertices[v1]<<"\n";
    std::cout<<vset.vertices[v2]<<"\n";
    std::cout<<vset.vertices[v3]<<"\n";*/
    Vector3f e1 = _scene->vertices[v2] - _scene->vertices[v1];
    Vector3f e2 = _scene->vertices[v3] - _scene->vertices[v1];

    Vector3f pvec = cross(ray.direction, e2);
    Real det = dot(e1, pvec);
    if (det == 0) return -1;
    Real invDet = 1.0 / det;
    Vector3f tvec = ray.origin - _scene->vertices[v1];
    Real u = dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) return -1;
    Vector3f qvec = cross(tvec, e1);
    Real v = dot(ray.direction, qvec) * invDet;
    if (v < 0 || u + v > 1) return -1;
    Real t = dot(e2, qvec) * invDet;
    if (t < 0) return -1;
    return t;
}

Vector3f Triangle::getCenter() {
    return (_scene->vertices[v1] + _scene->vertices[v2] + _scene->vertices[v3]) / (Real)3;
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




Scene::~Scene()
{
    for (auto it = meshes.begin(); it != meshes.end(); ++it)
        delete it->second;
}


