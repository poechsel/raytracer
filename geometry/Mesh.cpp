#include "Mesh.h"

Mesh::Mesh(Scene *sc): is_transformed(false), material(0), _scene(sc)
{
    //ctor
}

void Mesh::addVertex(Real x, Real y, Real z) {
    this->addVertex(Vector3f(x, y, z));
}
void Mesh::addVertex(Vector3f v) {
    this->vertices.push_back(_scene->getVerticesNumber());
    _scene->appendVertices(v);

}

void Mesh::addFace (uint v1, uint v2, uint v3, Vector3f n){
    this->triangles.push_back(_scene->getTrianglesNumber());
    _scene->appendTriangle(Triangle(v1, v2, v3, n, _scene));
}
void Mesh::addFace (uint v1, uint v2, uint v3){
    this->triangles.push_back(_scene->getTrianglesNumber());
    _scene->appendTriangle(Triangle(v1, v2, v3, _scene));
}
void Mesh::setTransform(Matrix4f t) {
    transform = t;
    inverse_transform = t.inverse();
    transpose = t.inverse().transpose();
    is_transformed = true;
}


void Scene::appendMeshes(std::map<std::string, Mesh*> &others) {
    for (auto it = others.begin(); it != others.end(); ++it) {
        meshes[it->first] = (it->second);
        if (!meshes[it->first]->material) {
            meshes[it->first]->material = &_default_mat;
        }
    }
    for (auto it =meshes.begin(); it !=meshes.end(); ++it) {
            std::cout<<"manager->Mesh: "<<it->first<<" -> "<<&(it->second)<<"\n";
            std::cout<<"    Faces: "<<it->second->triangles.size()<<" Vertices: "<<it->second->vertices.size()<<"\n";
    }
}

void Mesh::applyTransform(){
    for (auto vertex : vertices) {
        _scene->vertices[vertex] = transform * _scene->vertices[vertex];
    }
    for (auto triangle : triangles) {
        _scene->triangles[triangle].recalculate();
    }
}

Real Mesh::intersect(const Ray &ray) {
    Ray new_ray = ray;
    if (!is_transformed)
        new_ray.transform(inverse_transform);
    Real t = -1;
    for (auto triangle : triangles) {
        Real temp = _scene->triangles[triangle].intersect(new_ray);
        if (temp > 0 && (t < 0 || temp < t)) {
            t = temp;
            normal = _scene->triangles[triangle].normal;
            hitten_triangle = triangle;
        }
    }
    normal.w = 0;
    if (!is_transformed) {
        normal = transpose * normal;
        normal.normalizeMe();
    }
    if (t > 0) {
        return t;
    }
    return -1;
}

Mesh::~Mesh()
{
    //dtor
}
