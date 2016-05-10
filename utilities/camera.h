#ifndef CAMERA_H
#define CAMERA_H

#include "../includes.h"
#include "Ray.h"
class Camera
{
    public:
        Camera();
        virtual ~Camera();
        Real width;
        Real height;
        Real ratio;
        Real fov;
        Ray shoot(int x, int y);
        void setMatrixCoord(Real w, uint i, uint j){
            _matrix.set(w, i, j);
        }
    protected:
        Matrix4f _matrix;
    private:
};

#endif // CAMERA_H
