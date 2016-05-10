#include "camera.h"

Camera::Camera()
{
    //ctor
}

Ray Camera::shoot(int x, int y){
    float p_x = (2.f * ((x + 0.5f) / width) - 1) * std::tan(fov/2.f) * ratio;
    float p_y = (1.f - 2.f * ((y + 0.5f) / height)) * std::tan(fov/2.f);
    Ray ray (Vector3f(0, 0, 0), Vector3f(p_x, p_y, -1));
    ray.transform(_matrix);
   /* Matrix4f test = perspective(width, height, 0.1, 100, fov);
    Ray r2(Vector3f(0, 0, 0), Vector3f((x-0.5)/width, (y-0.5)/height, -1));
    Matrix4f result = test;
    r2.transform(result);*/
    Ray r2(Vector3f(0, 0, 0), Vector3f((x-0.5)/width, (y-0.5)/height, -1));
    Matrix4f result = _matrix;
    r2.transform(result);
    //std::cout<<r2.origin<<" | "<<r2.direction<<"\n";
    //r2.origin = r2.origin - Vector3f(0, 2, 5);
    //r2.transform(_matrix);
    return ray;
    //std::cout<<ray.origin<<"\n";
    return ray;
}

Camera::~Camera()
{
    //dtor
}
