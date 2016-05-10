#include "matTransformations.h"

Matrix4f perspective(Real width, Real height, Real near, Real far, Real fov){
    Real ratio = width / height;
    Real range = far - near;
    Real tanHalfFOV = std::tan(fov/2.0);
    Matrix4f temp;
    temp.data[0] = 1.0 / (tanHalfFOV * ratio);
    temp.data[5] = 1.0 / tanHalfFOV;
    temp.data[10] = (-near - far) / range;
    temp.data[11] = -2.0 * far * near / range;
    temp.data[14] = -1.0;
    return temp;
}

Matrix4f orthographic(Real left, Real right, Real bottom, Real top, Real near, Real far) {
    Matrix4f temp;
    temp.data[0] = 2.0/(right - left);
    temp.data[5] = 2.0/(top - bottom);
    temp.data[10] = -2.0/(far-near);
    temp.data[3] = -(right+left)/(right-left);
    temp.data[7] = -(top+bottom)/(top-bottom);
    temp.data[11] = -(far+near)/(far-near);
    return temp;
}
