#ifndef CAMERA_H
#define CAMERA_H

#include "../includes.h"
#include "Ray.h"
class Camera
{
    public:
        Camera();
        virtual ~Camera();

        Ray shoot(int x, int y);
        void setMatrixCoord(Real w, uint i, uint j);

        Real width;
        Real height;
        Real ratio;
        Real fov;
    protected:
        Matrix4f _matrix;
    private:
};

#endif // CAMERA_H
