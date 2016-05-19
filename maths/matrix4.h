#ifndef MATRIX4_H_INCLUDED
#define MATRIX4_H_INCLUDED

#include "../includes.h"


template <typename T>
class Matrix4 {
public:
    T data [16];

    //set a digit, i -> line number, j -> col number
    void set(Real w, int i, int j) {
        data[i*4 + j] = w;
    }

    Matrix4 () {
        for (unsigned int i = 0; i < 16; i++) {
            data[i] = 0;
        }
        data [0] = 1;
        data [5] = 1;
        data [10] = 1;
        data [15] = 1;
    }
    virtual ~Matrix4 () {}

    void setIdentity() {
        for (unsigned int i = 0; i < 16; i++) {
            data[0] = 0;
        }
        data [0] = 1;
        data [5] = 1;
        data [10] = 1;
        data [15] = 1;
    }

    Matrix4<T> transpose() {
        Matrix4<T> temp;
        temp.data[0]=data[0];
        temp.data[1]=data[4];
        temp.data[2]=data[8];
        temp.data[3]=data[12];
        temp.data[4]=data[1];
        temp.data[5]=data[5];
        temp.data[6]=data[9];
        temp.data[7]=data[13];
        temp.data[8]=data[2];
        temp.data[9]=data[6];
        temp.data[10]=data[10];
        temp.data[11]=data[14];
        temp.data[12]=data[3];
        temp.data[13]=data[7];
        temp.data[14]=data[11];
        temp.data[15]=data[15];
        return temp;
    }
    Matrix4<T> inverse () {
        Matrix4<T> temp;
        T det;
        temp.data[0] = data[5]  * data[10] * data[15] -
                 data[5]  * data[11] * data[14] -
                 data[9]  * data[6]  * data[15] +
                 data[9]  * data[7]  * data[14] +
                 data[13] * data[6]  * data[11] -
                 data[13] * data[7]  * data[10];

        temp.data[4] = -data[4]  * data[10] * data[15] +
                  data[4]  * data[11] * data[14] +
                  data[8]  * data[6]  * data[15] -
                  data[8]  * data[7]  * data[14] -
                  data[12] * data[6]  * data[11] +
                  data[12] * data[7]  * data[10];

        temp.data[8] = data[4]  * data[9] * data[15] -
                 data[4]  * data[11] * data[13] -
                 data[8]  * data[5] * data[15] +
                 data[8]  * data[7] * data[13] +
                 data[12] * data[5] * data[11] -
                 data[12] * data[7] * data[9];

        temp.data[12] = -data[4]  * data[9] * data[14] +
                   data[4]  * data[10] * data[13] +
                   data[8]  * data[5] * data[14] -
                   data[8]  * data[6] * data[13] -
                   data[12] * data[5] * data[10] +
                   data[12] * data[6] * data[9];

        temp.data[1] = -data[1]  * data[10] * data[15] +
                  data[1]  * data[11] * data[14] +
                  data[9]  * data[2] * data[15] -
                  data[9]  * data[3] * data[14] -
                  data[13] * data[2] * data[11] +
                  data[13] * data[3] * data[10];

        temp.data[5] = data[0]  * data[10] * data[15] -
                 data[0]  * data[11] * data[14] -
                 data[8]  * data[2] * data[15] +
                 data[8]  * data[3] * data[14] +
                 data[12] * data[2] * data[11] -
                 data[12] * data[3] * data[10];

        temp.data[9] = -data[0]  * data[9] * data[15] +
                  data[0]  * data[11] * data[13] +
                  data[8]  * data[1] * data[15] -
                  data[8]  * data[3] * data[13] -
                  data[12] * data[1] * data[11] +
                  data[12] * data[3] * data[9];

        temp.data[13] = data[0]  * data[9] * data[14] -
                  data[0]  * data[10] * data[13] -
                  data[8]  * data[1] * data[14] +
                  data[8]  * data[2] * data[13] +
                  data[12] * data[1] * data[10] -
                  data[12] * data[2] * data[9];

        temp.data[2] = data[1]  * data[6] * data[15] -
                 data[1]  * data[7] * data[14] -
                 data[5]  * data[2] * data[15] +
                 data[5]  * data[3] * data[14] +
                 data[13] * data[2] * data[7] -
                 data[13] * data[3] * data[6];

        temp.data[6] = -data[0]  * data[6] * data[15] +
                  data[0]  * data[7] * data[14] +
                  data[4]  * data[2] * data[15] -
                  data[4]  * data[3] * data[14] -
                  data[12] * data[2] * data[7] +
                  data[12] * data[3] * data[6];

        temp.data[10] = data[0]  * data[5] * data[15] -
                  data[0]  * data[7] * data[13] -
                  data[4]  * data[1] * data[15] +
                  data[4]  * data[3] * data[13] +
                  data[12] * data[1] * data[7] -
                  data[12] * data[3] * data[5];

        temp.data[14] = -data[0]  * data[5] * data[14] +
                   data[0]  * data[6] * data[13] +
                   data[4]  * data[1] * data[14] -
                   data[4]  * data[2] * data[13] -
                   data[12] * data[1] * data[6] +
                   data[12] * data[2] * data[5];

        temp.data[3] = -data[1] * data[6] * data[11] +
                  data[1] * data[7] * data[10] +
                  data[5] * data[2] * data[11] -
                  data[5] * data[3] * data[10] -
                  data[9] * data[2] * data[7] +
                  data[9] * data[3] * data[6];

        temp.data[7] = data[0] * data[6] * data[11] -
                 data[0] * data[7] * data[10] -
                 data[4] * data[2] * data[11] +
                 data[4] * data[3] * data[10] +
                 data[8] * data[2] * data[7] -
                 data[8] * data[3] * data[6];

        temp.data[11] = -data[0] * data[5] * data[11] +
                   data[0] * data[7] * data[9] +
                   data[4] * data[1] * data[11] -
                   data[4] * data[3] * data[9] -
                   data[8] * data[1] * data[7] +
                   data[8] * data[3] * data[5];

        temp.data[15] = data[0] * data[5] * data[10] -
                  data[0] * data[6] * data[9] -
                  data[4] * data[1] * data[10] +
                  data[4] * data[2] * data[9] +
                  data[8] * data[1] * data[6] -
                  data[8] * data[2] * data[5];

        det = data[0] * temp.data[0] + data[1] * temp.data[4]
                + data[2] * temp.data[8] + data[3] * temp.data[12];

        if (det == 0)
            return temp;
        det = (T)1 / det;
        return det*temp;
    }


    Matrix4<T>& operator+=(Matrix4<T>& rhs) {
        for (unsigned int i = 0; i < 4; ++i){
            for (unsigned int j = 0; j < 4; ++j){
                data [i][j] += rhs.data [i][j];
            }
        }
        return *this;
    }
    Matrix4<T>& operator-=(Matrix4<T>& rhs) {
        for (unsigned int i = 0; i < 4; ++i){
            for (unsigned int j = 0; j < 4; ++j){
                data [i][j] -= rhs.data [i][j];
            }
        }
        return *this;
    }
    Matrix4<T>& operator*=(Matrix4<T>& rhs) {
        *this = (*this) * rhs;
        return *this;
    }
    template <typename Y>
    Matrix4<T>& operator*=(Y k) {
        for (unsigned int i = 0; i < 4; ++i){
            for (unsigned int j = 0; j < 4; ++j){
                data [i*4+j] *= k;
            }
        }
        return *this;
    }
    Matrix4<T>& operator= (const Matrix4<T> &rhs){
        for (unsigned int i = 0; i < 4; ++i){
            for (unsigned int j = 0; j < 4; ++j){
                data [i*4+j] = rhs.data [i*4+j];
            }
        }
        return *this;
    }
};

template <typename T>
Matrix4<T> operator*(Matrix4<T> a, T k) {
    Matrix4<T> temp = a;
    temp *= k;
    return temp;
}
template <typename T>
Matrix4<T> operator*(T k, Matrix4<T> a) {
    Matrix4<T> temp = a;
    temp *= k;
    return temp;
}
template <typename T>
Matrix4<T> operator+(Matrix4<T> a, Matrix4<T> b) {
    Matrix4<T> temp = a;
    temp += b;
    return temp;
}
template <typename T>
Matrix4<T> operator-(Matrix4<T> a, Matrix4<T> b) {
    Matrix4<T> temp = a;
    temp -= b;
    return temp;
}
template <typename T>
Matrix4<T> operator*(Matrix4<T> a, Matrix4<T> b) {
    Matrix4<T> temp;
    for (unsigned int i = 0; i < 4; ++i){
        for (unsigned int j = 0; j < 4; ++j){
            for (unsigned int k = 0; k < 4; ++k){
                temp.data[i*4+j] += a.data [i*4+k] * b.data [k*4+j];
            }
        }
    }
    return temp;
}
template <typename T>
Matrix4<T> translationMatrix (T x, T y, T z) {
    Matrix4<T> temp;
    temp.data[3] = x;
    temp.data[7] = y;
    temp.data[11] = z;
    return temp;
}
template <typename T>
Matrix4<T> scaleMatrix (T x, T y, T z) {
    Matrix4<T> temp;
    temp.data[0] = x;
    temp.data[5] = y;
    temp.data[10] = z;
    return temp;
}

//right handed
template<typename T>
Matrix4<T> rotationMatrix (T angle, T x, T y, T z) {
    T l = std::sqrt(x*x+y*y+z*z);
    x /= l;
    y /= l;
    z /= l;
    T c = std::cos (angle);
    T s = std::sin (angle);
    T t = (T)1 - c;
    Matrix4<T> temp;
    temp.data [0] = t*x*x + c;
    temp.data [5] = t*y*y + c;
    temp.data [10] = t*z*z + c;
    temp.data [1] = t*x*y - s*z;
    temp.data [4] = t*x*y + s*z;
    temp.data [2] = t*x*z + s*y;
    temp.data [8] = t*x*z - s*y;
    temp.data [6] = t*y*z - s*x;
    temp.data [9] = t*y*z + s*x;
    return temp;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix4<T>& m) {
    os<<'|'<<m.data[0]<<'\t'<<m.data[1]<<'\t'<<m.data[2]<<'\t'<<m.data[3]<<"|\n"
    <<'|'<<m.data[4]<<'\t'<<m.data[5]<<'\t'<<m.data[6]<<'\t'<<m.data[7]<<"|\n"
    <<'|'<<m.data[8]<<'\t'<<m.data[9]<<'\t'<<m.data[10]<<'\t'<<m.data[11]<<"|\n"
    <<'|'<<m.data[12]<<'\t'<<m.data[13]<<'\t'<<m.data[14]<<'\t'<<m.data[15]<<"|\n";
  return os;
}

#endif // MATRIX4_H_INCLUDED
