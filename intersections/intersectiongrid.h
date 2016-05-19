#ifndef INTERSECTIONGRID_H
#define INTERSECTIONGRID_H

#include "intersectionmethod.h"
#include <stack>
#include "intersections.h"
#include "../utilities/benchmark.h"

struct GridTraversalInformations {
    Vector3f dt;
    Vector3f t;
    Vector3f inc;
    Vector3<int> V;
};

class IntersectionGrid: public IntersectionMethod
{
    public:
        IntersectionGrid(Scene *scene, Real s, bool use_dfs = true);
        virtual void    build();
        virtual Real    intersect(Ray const &ray, uint *t_inter);
    protected:
        /* transforme la position d'un voxel en son index dans la grille*/
        int             getCaseIndex(Vector3<int> voxel);
        /* Créer la grille*/
        void            createGrid();
        /* Donne la case de la grille dans lequel est inscrit un point */
        Vector3<int>    convertToGridCoord(Vector3f pos);
        /* voxelize un triangle en utilisant un dfs*/
        void            voxelizeTriDfs(uint triangle);
        /* voxelize un triangle en parcourant toutes ses cases*/
        void            voxelizeTriSeq(uint triangle);
        /* effectue le test d'intersection entre un rayon et les triangles
        *  contenues dans un voxel */
        Real            intersectCase(Ray const &ray, Vector3<int> V, uint *tri);
        /* Traverse la grille selon un rayon et des informations de parcous*/
        Real            traversal(const Ray &ray, uint *tri,
                                  GridTraversalInformations &gti);
        /* initialise les informations de parcours*/
        GridTraversalInformations initTraversal(const Ray &ray, Vector3f &start);


        std::vector <std::vector<uint> >    _grid;
        std::vector <uint>                  _grid_cache;
        Vector3<int>                        _dimensions;
        Vector3<int>                        _offset;
        Real                                _size;
    private:
        bool _use_dfs;
};

#endif // INTERSECTIONGRID_H
