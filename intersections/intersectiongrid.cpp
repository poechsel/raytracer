#include "intersectiongrid.h"

IntersectionGrid::IntersectionGrid(Scene *scene, Real s, bool use_dfs):
    IntersectionMethod(scene)
{
    this->_use_dfs = use_dfs;
    this->_size = s;
    this->_offset = Vector3<int>(0, 0, 0);
}




void IntersectionGrid::build(int offset){
    this->createGrid();
    if (this->_use_dfs) {
        for (uint i = 0; i < this->_scene->triangles.size(); ++i) {
            this->voxelizeTriDfs(i);
        }
    } else {
        for (uint i = 0; i < this->_scene->triangles.size(); ++i) {
            this->voxelizeTriSeq(i);
        }
    }
}




Real IntersectionGrid::intersect(Ray const &ray, uint *t_inter){
    Real t_min, t_max;
    Vector3f of (this->_offset.x, this->_offset.y, this->_offset.z);
    Vector3f dim (this->_dimensions.x, this->_dimensions.y, this->_dimensions.z);
    Vector3f center =  ((Real) 2 * of + dim) * 0.5 * this->_size;
    Vector3f half_size = dim * 0.5 * this->_size;
    if (intersectionBoxRay(center, half_size, ray, &t_min, &t_max)){
        t_min = std::max(t_min, (Real) 0);
        Vector3f start = ray.origin + t_min * ray.direction;
        GridTraversalInformations gti = this->initTraversal(ray, start);
        Real t = this->traversal(ray, t_inter, gti);
        return t;
    }
    return -1;
}




GridTraversalInformations IntersectionGrid::initTraversal(const Ray &ray,
                                                          Vector3f &start)
{
    GridTraversalInformations gti;
    gti.V   = this->convertToGridCoord(start);
    gti.dt  = Vector3f(this->_size * 1.0/std::abs(ray.direction.x),
                        this->_size * 1.0/std::abs(ray.direction.y),
                        this->_size * 1.0/std::abs(ray.direction.z));
    gti.inc = Vector3f(0, 0, 0);
    gti.t   = Vector3f(0, 0, 0);
    if (ray.direction.x == 0) {
        gti.inc.x = 0;
        gti.t.x = this->_dimensions.x * this->_dimensions.x;
    } else if (ray.direction.x > 0) {
        gti.inc.x = 1;
        gti.t.x = ((Real)gti.V.x + 1.0 - start.x / this->_size) * gti.dt.x;
    } else {
        gti.inc.x = -1;
        gti.t.x = (start.x / this->_size - (Real)gti.V.x) * gti.dt.x;
    }
    if (ray.direction.y == 0) {
        gti.inc.y = 0;
        gti.t.y = this->_dimensions.y * this->_dimensions.y;
    } else if (ray.direction.y > 0) {
        gti.inc.y = 1;
        gti.t.y = ((Real)gti.V.y + 1.0 - start.y / this->_size) * gti.dt.y;
    } else {
        gti.inc.y = -1;
        gti.t.y = (start.y / this->_size - (Real)gti.V.y) * gti.dt.y;
    }
    if (ray.direction.z == 0) {
        gti.inc.z = 0;
        gti.t.z = this->_dimensions.z * this->_dimensions.z;
    } else if (ray.direction.z > 0) {
        gti.inc.z = 1;
        gti.t.z = ((Real)gti.V.z + 1.0 - start.z / this->_size) * gti.dt.z;
    } else {
        gti.inc.z = -1;
        gti.t.z = (start.z / this->_size - (Real)gti.V.z) * gti.dt.z;
    }
    return gti;
}




/* contrairement à ce qu'envisage le papier "A fast voxel Traversal
  Algorithm", il semblerait que tester si l'intersection se situe dans le
  voxel courant ne fonctionne pas, et qu'au contraire ajouter ce test ne
  donne pas le résultat voulu
*/
Real IntersectionGrid::traversal(const Ray &ray, uint *tri,
                                 GridTraversalInformations &gti) {
    Real t_temp;
    do{
        t_temp = this->intersectCase(ray, gti.V, tri);
        if (0 <= t_temp )
                return t_temp;
        if (gti.t.x <= gti.t.y && gti.t.x <= gti.t.z) {
            gti.t.x += gti.dt.x;
            gti.V.x += gti.inc.x;
            if ( gti.V.x < this->_offset.x
                || this->_offset.x + this->_dimensions.x <= gti.V.x)
                break;
        }
        else if (gti.t.y <= gti.t.x && gti.t.y <= gti.t.z) {
            gti.t.y += gti.dt.y;
            gti.V.y += gti.inc.y;
            if ( gti.V.y < this->_offset.y
                || this->_offset.y + this->_dimensions.y <= gti.V.y)
                break;
        } else {
            gti.t.z += gti.dt.z;
            gti.V.z += gti.inc.z;
            if ( gti.V.z < this->_offset.z
                || this->_offset.z + this->_dimensions.z <= gti.V.z)
                break;
        }
    } while(true);
    return -1;
}





Real IntersectionGrid::intersectCase(Ray const &ray, Vector3<int> V, uint *t_inter) {
    Real t = -1;
    int index = this->getCaseIndex(V - this->_offset);
    if (index < 0 || index >= this->_grid.size()){
        return t;
    }
    for (uint i = 0; i < this->_grid[index].size(); ++i){
        Triangle &triangle = this->_scene->triangles[this->_grid[index][i]];
        Real t_temp = intersectionMoller(_scene, triangle, ray);
        if (t_temp >= 0) {
            if (t < 0 || t_temp < t) {
                t = t_temp;
                *t_inter = this->_grid[index][i];
            }
        }
    }
    return t;
}




void IntersectionGrid::voxelizeTriSeq(uint tri) {
    Triangle &triangle = this->_scene->triangles[tri];
    BoundingBox bb (this->_scene, &triangle);
    Vector3<int> m = this->convertToGridCoord(bb.m);
    Vector3<int> M = this->convertToGridCoord(bb.M) + Vector3<int>(1);

    for (int x = m.x; x < M.x; ++x) {
        for (int y = m.y; y < M.y; ++y) {
            for (int z = m.z; z < M.z; ++z) {
                Vector3f current (x, y, z);
                Vector3<int> ccase = Vector3<int>(x, y, z) - this->_offset;
                uint current_index = this->getCaseIndex(ccase);
                if (intersectionBoxTri(this->_scene,
                                       (current + Vector3f(0.5)) * this->_size,
                                       Vector3f(this->_size/2),
                                       triangle)) {
                    this->_grid[current_index].push_back(tri);
                }
            }
        }
    }
}




void IntersectionGrid::voxelizeTriDfs(uint tri) {
    Triangle &triangle = this->_scene->triangles[tri];
    std::stack<Vector3<int>> next;
    Vector3f v1 = this->_scene->vertices[triangle.v1];
    Vector3<int> current = this->convertToGridCoord(v1) - this->_offset;
    next.push(current);
    _grid_cache[this->getCaseIndex(current)] = tri + 1;
    while (next.size()) {
        current = next.top();
        next.pop();
        Vector3f currentf(current.x, current.y, current.z);
        Vector3f of(this->_offset.x, this->_offset.y, this->_offset.z);
        if (intersectionBoxTri(this->_scene,
                               (currentf + of + Vector3f(0.5)) * this->_size,
                               Vector3f(this->_size/2),
                               triangle)) {
            this->_grid[this->getCaseIndex(current)].push_back(tri);

            for (int dx = -1; dx <= 1; ++dx) {
                if (current.x + dx < 0
                    || this->_dimensions.x <= current.x + dx){
                    continue;
                }
                for (int dy = -1; dy <= 1; ++dy) {
                    if (current.y + dy < 0
                        || this->_dimensions.y <= current.y + dy) {
                        continue;
                    }
                    for (int dz = -1; dz <= 1; ++dz) {
                        if (current.z + dz < 0
                            || this->_dimensions.z <= current.z + dz) {
                            continue;
                        }
                        Vector3<int> nvoxel = current + Vector3<int>(dx, dy, dz);
                        uint n_index = this->getCaseIndex(nvoxel);
                        if (this->_grid_cache[n_index] != tri + 1) {
                            next.push(nvoxel);
                            this->_grid_cache[n_index] = tri+1;
                        }
                    }
                }
            }
        }
    }
}




void IntersectionGrid::createGrid() {
    BoundingBox bb;
    for (auto &triangle : this->_scene->triangles) {
        bb.expand(this->_scene, &triangle);
    }
    Vector3<int> m = this->convertToGridCoord(bb.m);
    Vector3<int> M = this->convertToGridCoord(bb.M);

    //ici on choisi de faire une grille carré
    this->_offset = m;
    Vector3<int> temp = M + Vector3<int>(1, 1, 1) - m;
    int tm = std::max(std::max(temp.x, temp.y), temp.z);
    this->_dimensions = Vector3<int>(tm);

    uint size_grid = this->_dimensions.x*this->_dimensions.y*this->_dimensions.z;
    _grid.resize(size_grid);
    _grid_cache.resize(size_grid);
}





int IntersectionGrid::getCaseIndex(Vector3<int> voxel) {
    return voxel.x
           + voxel.y * this->_dimensions.x
           + voxel.z * this->_dimensions.x * this->_dimensions.y;
}




Vector3<int> IntersectionGrid::convertToGridCoord(Vector3f pos) {
    return Vector3<int>(std::floor(pos.x/this->_size),
                         std::floor(pos.y/this->_size),
                         std::floor(pos.z/this->_size));
}
