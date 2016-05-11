#ifndef INTERSECTIONGRID_H
#define INTERSECTIONGRID_H

#include "intersectionmethod.h"
#include <stack>
#include "intersections.h"
#include "../utilities/benchmark.h"

class IntersectionGrid: public IntersectionMethod
{
    public:
        IntersectionGrid(Scene *scene, Real s, bool use_dfs = true);
        virtual void build();
        virtual Real intersect(Ray const &ray, uint *t_inter);
    protected:
        std::vector <std::vector<uint> >    _grid;
        std::vector <uint>                  _grid_cache;
        Vector3<int>                       _dimensions;
        Vector3<int>                        _offset;
        Real                                _size;


        int getCaseIndex(Vector3<int> voxel);
        void createGrid();
        Vector3<int> convertToGridCoord(Vector3f pos);
        void voxelizeTriDfs(uint triangle);
        void voxelizeTriSeq(uint triangle);
        Real intersectCase(Ray const &ray, Vector3<int> V, uint *tri);
        Real traversal(const Ray &ray, Vector3f start, uint *tri);
    private:
        bool _use_dfs;
};

#endif // INTERSECTIONGRID_H
