#include "intersections.h"


inline Real sign(Real a){
    return (a < 0)? -1 : 1;
}


Real intersectionMoller(Scene *scene,
                        Triangle &triangle,
                        const Ray &ray) {
    Vector3f P = cross(ray.direction, triangle.e2);

    Real det = dot(triangle.e1, P);
    if (det < FLT_EPSILON && -FLT_EPSILON < det) {
        return -1;
    }
    Vector3f  T = ray.origin - scene->vertices[triangle.v1];
    Real invDet = 1.f/det;

    Real u = dot(T, P) * invDet;
    if (u < 0 || u > 1) {
        return -1;
    }
    Vector3f Q = cross(T, triangle.e1);
    Real v = dot(ray.direction, Q) * invDet;
    if (v < 0 || u + v > 1) {
        return -1;
    }
    Real t = dot(triangle.e2, Q) * invDet;
    if (t < 0) {
        return -1;
    }
    return t;
}

Real intersectionNaive(Scene *scene,
                       Triangle &tri,
                       const Ray &ray){
    Real dn = dot(tri.normal, ray.direction);
    if (-FLT_EPSILON < dn && dn < FLT_EPSILON) {
        return -1;
    }
    Real t = - dot(tri.normal, ray.origin - scene->vertices[tri.v1]) / dn;
    if (t < 0){
        return -1;
    }
    Vector3f P = ray.origin + t * ray.direction;
    Vector3f temp = P - scene->vertices[tri.v1];
    Vector3f AP0P1P2 = cross(tri.e1, tri.e2);
    Real invAP0P1P2 = 1.f / AP0P1P2.length();
    Vector3f triU = cross(P - scene->vertices[tri.v3], temp);
    Real u = triU.length() * invAP0P1P2 * sign(dot(AP0P1P2, triU));
    if (u < 0 || u > 1) {
        return -1;
    }
    Vector3f triV = cross(temp, P - scene->vertices[tri.v2]);
    Real v = triV.length() * invAP0P1P2  * sign(dot(AP0P1P2, triV));
    if (v < 0 || u + v > 1) {
        return -1;
    }
    return t;
}

bool intersectionBoxRay(Vector3f center, Vector3f half_size,
                        const Ray &ray,
                        Real *t_min, Real *t_max){
    Real t1 = (center.x - half_size.x - ray.origin.x ) / ray.direction.x;
    Real t2 = (center.x + half_size.x - ray.origin.x ) / ray.direction.x;
    Real tmin = std::min(t1, t2);
    Real tmax = std::max(t1, t2);

    t1 = (center.y - half_size.y - ray.origin.y ) / ray.direction.y;
    t2 = (center.y + half_size.y - ray.origin.y ) / ray.direction.y;
    tmin = std::max(tmin, std::min(t1, t2));
    tmax = std::min(tmax, std::max(t1, t2));

    t1 = (center.z - half_size.z - ray.origin.z ) / ray.direction.z;
    t2 = (center.z + half_size.z - ray.origin.z ) / ray.direction.z;
    tmin = std::max(tmin, std::min(t1, t2));
    tmax = std::min(tmax, std::max(t1, t2));
    if (tmax >= std::max(tmin, (Real)0)) {
        *t_max = tmax;
        *t_min = tmin;
        return true;
    }
    return false;
}


bool intersectionBoxTri(const Scene *scene,
                        Vector3f center, Vector3f half_size,
                        const Triangle &triangle) {
    Vector3f v1 = scene->vertices[triangle.v1] - center;
    Vector3f v2 = scene->vertices[triangle.v2] - center;
    Vector3f v3 = scene->vertices[triangle.v3] - center;

    Vector3f m (std::min(std::min(v1.x, v2.x), v3.x),
                std::min(std::min(v1.y, v2.y), v3.y),
                std::min(std::min(v1.z, v2.z), v3.z));
    Vector3f M (std::max(std::max(v1.x, v2.x), v3.x),
                std::max(std::max(v1.y, v2.y), v3.y),
                std::max(std::max(v1.z, v2.z), v3.z));

    if (M.x < - half_size.x || half_size.x < m.x)
        return false;
    if (M.y < - half_size.y || half_size.y < m.y)
        return false;
    if (M.z < - half_size.z || half_size.z < m.z)
        return false;

    Vector3f e1 = v2 - v1;
    Vector3f e2 = v3 - v1;
    Vector3f e3 = v3 - v2;
    Vector3f normal = cross(e1, e2);

    // cube on tri normal
    Real proj = dot(normal, v1);
    Real proj_max = std::abs(normal.x) * half_size.x
                    + std::abs(normal.y) * half_size.y
                    + std::abs(normal.z) * half_size.z;
    if (proj < -proj_max || proj_max < proj)
        return false;

    //on teste les 9 derniers axes
    //tous d'abord, (1, 0, 0) avec les arêtes du triangle
    //e1
    proj_max = std::abs(e1.z) * half_size.y + std::abs(e1.y) * half_size.z;
    Real p1 = v1.z * v2.y - v1.y * v2.z;
    Real p2 = -e1.z * v3.y + e1.y * v3.z;
    if (std::min(p1, p2) > proj_max || std::max(p1, p2) < -proj_max)
        return false;
    //e2
    proj_max = std::abs(e2.z) * half_size.y + std::abs(e2.y) * half_size.z;
    p1 = v1.z * v3.y - v1.y * v3.z;
    p2 = -e2.z * v2.y + e2.y * v2.z;
    if (std::min(p1, p2) > proj_max || std::max(p1, p2) < -proj_max)
        return false;
    //e3
    proj_max = std::abs(e3.z) * half_size.y + std::abs(e3.y) * half_size.z;
    p1 = v2.z * v3.y - v2.y * v3.z;
    p2 = -e3.z * v1.y + e3.y * v1.z;
    if (std::min(p1, p2) > proj_max || std::max(p1, p2) < -proj_max)
        return false;

    //(0, 1, 0)
    //e1
    proj_max = std::abs(e1.z) * half_size.x + std::abs(e1.x) * half_size.z;
    p1 = v1.x * v2.z - v1.z * v2.x;
    p2 = e1.z * v3.x - e1.x * v3.z;
    if (std::min(p1, p2) > proj_max || std::max(p1, p2) < -proj_max)
        return false;
    //e2
    proj_max = std::abs(e2.z) * half_size.x + std::abs(e2.x) * half_size.z;
    p1 = v1.x * v3.z - v1.z * v3.x;
    p2 = e2.z * v2.x - e2.x * v2.z;
    if (std::min(p1, p2) > proj_max || std::max(p1, p2) < -proj_max)
        return false;
    //e3
    proj_max = std::abs(e3.z) * half_size.x + std::abs(e3.x) * half_size.z;
    p1 = v2.x * v3.z - v2.z * v3.x;
    p2 = e3.z * v1.x - e3.x * v1.z;
    if (std::min(p1, p2) > proj_max || std::max(p1, p2) < -proj_max)
        return false;

    //(0, 0, 1)
    //e1
    proj_max = std::abs(e1.y) * half_size.x + std::abs(e1.x) * half_size.y;
    p1 = v1.y * v2.x - v1.x * v2.y;
    p2 = -e1.y * v3.x + e1.x * v3.y;
    if (std::min(p1, p2) > proj_max || std::max(p1, p2) < -proj_max)
        return false;
    //e2
    proj_max = std::abs(e2.y) * half_size.x + std::abs(e2.x) * half_size.y;
    p1 = v1.y * v3.x - v1.x * v3.y;
    p2 = -e2.y * v2.x + e2.x * v2.y;
    if (std::min(p1, p2) > proj_max || std::max(p1, p2) < -proj_max)
        return false;
    //e3
    proj_max = std::abs(e3.y) * half_size.x + std::abs(e3.x) * half_size.y;
    p1 = v2.y * v3.x - v2.x * v3.y;
    p2 = -e3.y * v1.x + e3.x * v1.y;
    if (std::min(p1, p2) > proj_max || std::max(p1, p2) < -proj_max)
        return false;

    //si aucun axe séparateur n'est trouvé
    return true;
}


