#include "camera.h"

Camera::Camera()
{
    //ctor
}

/* procéde en deux temps: d'abord envoie un rayon pour une caméra perspective
*  dirigé en +z, puis transforme ce rayon suivant la matrice de transformation
*  de la caméra
*/
Ray Camera::shoot(int x, int y){
    float p_x = (2.f * ((x + 0.5f) / width) - 1) * std::tan(fov/2.f) * ratio;
    float p_y = (1.f - 2.f * ((y + 0.5f) / height)) * std::tan(fov/2.f);
    Ray ray (Vector3f(0, 0, 0), Vector3f(p_x, p_y, -1));
    ray.transform(_matrix);
    return ray;
}


void Camera::setMatrixCoord(Real w, uint i, uint j){
    _matrix.set(w, i, j);
}

Camera::~Camera()
{
    //dtor
}
