#include "intersectiongrid.h"

IntersectionGrid::IntersectionGrid(Scene *scene, Real s, bool use_dfs):
    IntersectionMethod(scene)
{
    this->_use_dfs = use_dfs;
    this->_size = s;
    this->_offset = Vector3<int>(0, 0, 0);
}

void IntersectionGrid::build(){
    this->createGrid();
    std::cout<<"La grille est de dimensions: "<<_dimensions<<" offset "<<_offset<<"\n";
    if (this->_use_dfs) {
        for (uint i = 0; i < this->_scene->triangles.size(); ++i) {
            this->voxelizeTriDfs(i);
        }
    } else {
        for (uint i = 0; i < this->_scene->triangles.size(); ++i) {
            this->voxelizeTriSeq(i);
        }
    }
   /*ULARGE_INTEGER tb = getTime();
    for (uint i = 0; i < this->_scene->triangles.size(); ++i) {
        //std::cout<<i<<"\n";
        this->voxelizeTriDfs(i);
    }
    std::cout<<getTimeElapsed(tb)<<"\n";

    std::vector<std::vector<uint> > temp;

    for (uint i = 0; i < this->_grid.size(); ++i) {
        //std::cout<<i<<" "<<this->_grid[i].size()<<"\n"
        temp.push_back(this->_grid[i]);
        this->_grid[i] = std::vector<uint>();
    }

    tb = getTime();
    for (uint i = 0; i < this->_scene->triangles.size(); ++i) {
        //std::cout<<i<<"\n";
        this->voxelizeTri2(i);
    }
    std::cout<<getTimeElapsed(tb)<<"\n";//*/

}

Real IntersectionGrid::intersect(Ray const &ray, uint *t_inter){
    Real t_min, t_max;
    Vector3f of (this->_offset.x, this->_offset.y, this->_offset.z);
    Vector3f dim (this->_dimensions.x, this->_dimensions.y, this->_dimensions.z);
    Vector3f center =  ((Real) 2 * of + dim) * 0.5 * this->_size;
    Vector3f half_size = dim * 0.5 * this->_size;
    if (intersectionBoxRay(center, half_size, ray, &t_min, &t_max)){
        Real t = this->traversal(ray, ray.origin + std::max(t_min, (Real)0) * ray.direction, t_inter);
        return t;
    }
    return -1;
}

Real IntersectionGrid::traversal(const Ray &ray, Vector3f start, uint *tri) {
    Vector3<int> V = this->convertToGridCoord(start);
    Vector3f dt (this->_size * 1.0/std::abs(ray.direction.x), this->_size * 1.0/std::abs(ray.direction.y), this->_size * 1.0/std::abs(ray.direction.z));
    Vector3f inc (0, 0, 0);
    Vector3f t (0, 0, 0);

    if (ray.direction.x == 0) {
        inc.x = 0;
        t.x = this->_dimensions.x * this->_dimensions.x;
    } else if (ray.direction.x > 0) {
        inc.x = 1;
        t.x = ((Real)V.x + 1.0 - start.x / this->_size) * dt.x;
    } else {
        inc.x = -1;
        t.x = (start.x / this->_size - (Real)V.x) * dt.x;
    }
    if (ray.direction.y == 0) {
        inc.y = 0;
        t.y = this->_dimensions.y * this->_dimensions.y;
    } else if (ray.direction.y > 0) {
        inc.y = 1;
        t.y = ((Real)V.y + 1.0 - start.y / this->_size) * dt.y;
    } else {
        inc.y = -1;
        t.y = (start.y / this->_size - (Real)V.y) * dt.y;
    }
    if (ray.direction.z == 0) {
        inc.z = 0;
        t.z = this->_dimensions.z * this->_dimensions.z;
    } else if (ray.direction.z > 0) {
        inc.z = 1;
        t.z = ((Real)V.z + 1.0 - start.z / this->_size) * dt.z;
    } else {
        inc.z = -1;
        t.z = (start.z / this->_size - (Real)V.z) * dt.z;
    }
    Real t_temp;
    bool b = true;
    bool end = false;
    /* contrairement à ce qu'envisage le papier "A fast voxel Traversal Algorithm", il semblerait que tester si l'intersection se situe dans le voxel courant ne fonctionne pas, et qu'au contraire ajouter ce test ne donne pas le résultat voulu
    */
    do{
        t_temp = this->intersectCase(ray, V, tri);
        if (0 <= t_temp )// && V == this->convertToGridCoord(ray.origin + t_temp * ray.direction))
                return t_temp;
        if (t.x <= t.y && t.x <= t.z) {
            t.x += dt.x;
            V.x += inc.x;
            if (!(this->_offset.x <= V.x && V.x < this->_offset.x + this->_dimensions.x))
                break;
        }
        else if (t.y <= t.x && t.y <= t.z) {
            t.y += dt.y;
            V.y += inc.y;
            if (!(this->_offset.y <= V.y && V.y < this->_offset.y + this->_dimensions.y))
                break;
        } else {
            t.z += dt.z;
            V.z += inc.z;
            if (!(this->_offset.z <= V.z && V.z < this->_offset.z + this->_dimensions.z))
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
        Real t_temp = intersectionMoller(_scene, this->_scene->triangles[this->_grid[index][i]], ray);
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
    for (int x = this->_offset.x; x < this->_dimensions.x + this->_offset.x; ++x) {
        for (int y = this->_offset.y; y < this->_dimensions.y + this->_offset.y; ++y) {
            for (int z = this->_offset.z; z < this->_dimensions.z + this->_offset.z; ++z) {
                Vector3f current (x, y, z);
                if (intersectionBoxTri(this->_scene, (current + Vector3f(0.5)) * this->_size, Vector3f(this->_size/2), this->_scene->triangles[tri])) {
                    this->_grid[this->getCaseIndex(Vector3<int>(x, y, z) - this->_offset)].push_back(tri);
                }
            }
        }
    }
}

void IntersectionGrid::voxelizeTriDfs(uint tri) {
    std::stack<Vector3<int>> next;
    Vector3<int> current = this->convertToGridCoord(this->_scene->vertices[this->_scene->triangles[tri].v1]) - this->_offset;
    next.push(current);
    _grid_cache[this->getCaseIndex(current)] = tri + 1;
    while (next.size()) {
        current = next.top();
        next.pop();
        Vector3f currentf(current.x, current.y, current.z);
        Vector3f of(this->_offset.x, this->_offset.y, this->_offset.z);
        if (intersectionBoxTri(this->_scene, (currentf + of + Vector3f(0.5)) * this->_size, Vector3f(this->_size/2), _scene->triangles[tri])) {
            this->_grid[this->getCaseIndex(current)].push_back(tri);
            for (int dx = -1; dx <= 1; ++dx) {
                if (current.x + dx < 0 || this->_dimensions.x <= current.x + dx){
                    continue;
                }
                for (int dy = -1; dy <= 1; ++dy) {
                    if (current.y + dy < 0 || this->_dimensions.y <= current.y + dy) {
                        continue;
                    }
                    for (int dz = -1; dz <= 1; ++dz) {
                        if (current.z + dz < 0 || this->_dimensions.z <= current.z + dz) {
                            continue;
                        }
                        Vector3<int> nvoxel = current + Vector3<int>(dx, dy, dz);
                        if (this->_grid_cache[this->getCaseIndex(nvoxel)] != tri + 1) {
                            next.push(nvoxel);
                            this->_grid_cache[this->getCaseIndex(nvoxel)] = tri+1;
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
    this->_offset = m;
    Vector3<int> temp = M + Vector3<int>(1, 1, 1) - m;
    int tm = std::max(std::max(temp.x, temp.y), temp.z);
    this->_dimensions = Vector3<int>(temp.x, temp.y, temp.z);
    //on fait une grille carré, mais elle pourrait être rectangulaire
    this->_dimensions = Vector3<int>(tm);
    _grid.resize(this->_dimensions.x * this->_dimensions.y * this->_dimensions.z);
    _grid_cache.resize(this->_dimensions.x * this->_dimensions.y * this->_dimensions.z);
}

int IntersectionGrid::getCaseIndex(Vector3<int> voxel) {
    return voxel.x + voxel.y * this->_dimensions.x  + voxel.z * this->_dimensions.x * this->_dimensions.y;
}

Vector3<int> IntersectionGrid::convertToGridCoord(Vector3f pos) {
    return Vector3<int>(std::floor(pos.x/this->_size),
                         std::floor(pos.y/this->_size),
                         std::floor(pos.z/this->_size));
}
