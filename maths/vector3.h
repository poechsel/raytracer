#ifndef VECTOR3_H_INCLUDED
#define VECTOR3_H_INCLUDED

#include "../includes.h"

template <typename T>
class Vector3 {
    public:
        T x;
        T y;
        T z;
        T w;

        Vector3<T> (T x, T y, T z, T w = 1) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }
        Vector3<T> (): Vector3<T>(0, 0, 0) {
        }
        Vector3<T> (T a): Vector3<T>(a, a, a) {
        }

        virtual ~Vector3<T> (){}

        void normalizeMe() {
            T l = this->length();
            x /= l;
            y /= l;
            z /= l;
        }
        Vector3<T> normalize() {
            T l = this->length();
            //false because why normalizing a point?
            return Vector3<T> (x / l, y / l, z / l, false);
        }
        inline T length() {
            return std::sqrt(this->squareLength());
        }
        inline T squareLength() {
            return x*x+y*y+z*z;
        }
        void makePosition() {
            this-> w = 1;
        }
        void makeDirection() {
            this->w  = 0;
        }
        Vector3<T>& operator+=(Vector3<T>& rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }
        Vector3<T>& operator-=(Vector3<T>& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
        template <typename Y>
        Vector3<T>& operator*=(Y& rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }
        template <typename Y>
        Vector3<T>& operator/=(Y& rhs) {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            return *this;
        }
        inline T dotProduct (Vector3<T> rhs) {
            return x*rhs.x + y*rhs.y + z*rhs.z;
        }
        T operator [](int i) const  {
            if (i == 0)
                return x;
            if (i == 1)
                return y;
            if (i == 2)
                return z;
            return w;
        }
        T & operator [](int i)    {
            if (i == 0)
                return x;
            if (i == 1)
                return y;
            if (i == 2)
                return z;
            return w;
        }
    private:
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& obj) {
  os<<obj.x<<" "<<obj.y<<" "<<obj.z;
  return os;
}
template <typename T>
inline Vector3<T> operator+(Vector3<T> a, Vector3<T> b) {
    return Vector3<T>(a.x+b.x, a.y+b.y, a.z+b.z);
}
template <typename T>
inline Vector3<T> operator-(Vector3<T> a, Vector3<T> b) {
    return Vector3<T>(a.x-b.x, a.y-b.y, a.z-b.z);
}
template <typename T>
inline Vector3<T> operator*(Vector3<T> a, T k) {
    return Vector3<T>(a.x*k, a.y*k, a.z*k);
}
template <typename T>
inline Vector3<T> operator*(T k, Vector3<T> a) {
    return Vector3<T>(a.x*k, a.y*k, a.z*k);
}
template <typename T, typename Y>
inline Vector3<T> operator/(Vector3<T> a, Y k) {
    return Vector3<T>(a.x/k, a.y/k, a.z/k);
}
template <typename T, typename Y>
inline Vector3<T> operator/(Y k, Vector3<T> a) {
    return Vector3<T>(a.x/k, a.y/k, a.z/k);
}

template <typename T>
inline T dot (Vector3<T> &a, Vector3<T> &b) {
    return a.x*b.x+a.y*b.y+a.z*b.z;
}
template <typename T>
inline T dot (const Vector3<T> &a, const Vector3<T> &b) {
    return a.x*b.x+a.y*b.y+a.z*b.z;
}
template <typename T>
inline Vector3<T> cross(Vector3<T> &b, Vector3<T> &c){
    return Vector3<T> (b.y*c.z-b.z*c.y, b.z*c.x-b.x*c.z, b.x*c.y-b.y*c.x, false);
}
template <typename T>
inline Vector3<T> cross(const Vector3<T> &b, Vector3<T> &c){
    return Vector3<T> (b.y*c.z-b.z*c.y, b.z*c.x-b.x*c.z, b.x*c.y-b.y*c.x, false);
}
template <typename T>
inline Vector3<T> cross(Vector3<T> &b, const Vector3<T> &c){
    return Vector3<T> (b.y*c.z-b.z*c.y, b.z*c.x-b.x*c.z, b.x*c.y-b.y*c.x, false);
}
template <typename T>
inline Vector3<T> cross(const Vector3<T> &b, const Vector3<T> &c){
    return Vector3<T> (b.y*c.z-b.z*c.y, b.z*c.x-b.x*c.z, b.x*c.y-b.y*c.x, false);
}
template <typename T>
inline bool operator==(Vector3<T> a, Vector3<T> b) {
    return a.x = b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}
template <typename T>
inline bool operator!=(Vector3<T> a, Vector3<T> b) {
    return !(a==b);
}



template <class T>
class Matrix4;
template <typename T>
Vector3<T> operator*(Matrix4<T> &mat, Vector3<T> &b) {
    Vector3<T> temp;
    temp.x = mat.data[0]*b.x+mat.data[1]*b.y+mat.data[2]*b.z+mat.data[3]*b.w;
    temp.y = mat.data[4]*b.x+mat.data[5]*b.y+mat.data[6]*b.z+mat.data[7]*b.w;
    temp.z = mat.data[8]*b.x+mat.data[9]*b.y+mat.data[10]*b.z+mat.data[11]*b.w;
    temp.w = mat.data[12]*b.x+mat.data[13]*b.y+mat.data[14]*b.z+mat.data[15]*b.w;
    return temp;
}






template <typename T>
class Vector2 {
    public:
        T x;
        T y;

        Vector2<T> (T x = 0, T y = 0) {
            this->x = x;
            this->y = y;
        }

        virtual ~Vector2<T> (){}

        void normalizeMe() {
            T l = this->length();
            x /= l;
            y /= l;
        }
        Vector2<T> normalize() {
            T l = this->length();
            //false because why normalizing a point?
            return Vector2<T> (x / l, y / l);
        }
        inline T length() {
            return std::sqrt(this->squareLength());
        }
        inline T squareLength() {
            return x*x+y*y;
        }
        Vector2<T>& operator+=(Vector2<T>& rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }
        Vector2<T>& operator-=(Vector2<T>& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }
        template <typename Y>
        Vector2<T>& operator*=(Y& rhs) {
            x *= rhs;
            y *= rhs;
            return *this;
        }
        template <typename Y>
        Vector2<T>& operator/=(Y& rhs) {
            x /= rhs;
            y /= rhs;
            return *this;
        }
        inline T dotProduct (Vector2<T> rhs) {
            return x*rhs.x + y*rhs.y;
        }
    private:
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& obj) {
  os<<obj.x<<" "<<obj.y;
  return os;
}
template <typename T>
inline Vector2<T> operator+(Vector2<T> a, Vector2<T> b) {
    return Vector2<T>(a.x+b.x, a.y+b.y);
}
template <typename T>
inline Vector2<T> operator-(Vector2<T> a, Vector2<T> b) {
    return Vector2<T>(a.x-b.x, a.y-b.y);
}
template <typename T>
inline Vector2<T> operator*(Vector2<T> a, T k) {
    return Vector2<T>(a.x*k, a.y*k);
}
template <typename T>
inline Vector2<T> operator*(T k, Vector2<T> a) {
    return Vector2<T>(a.x*k, a.y*k);
}
template <typename T, typename Y>
inline Vector2<T> operator/(Vector2<T> a, Y k) {
    return Vector2<T>(a.x/k, a.y/k);
}
template <typename T, typename Y>
inline Vector2<T> operator/(Y k, Vector2<T> a) {
    return Vector2<T>(a.x/k, a.y/k);
}

template <typename T>
inline T dot (Vector2<T> &a, Vector2<T> &b) {
    return a.x*b.x+a.y*b.y;
}
template <typename T>
inline T dot (const Vector2<T> &a, const Vector2<T> &b) {
    return a.x*b.x+a.y*b.y;
}
template <typename T>
inline bool operator==(Vector2<T> a, Vector2<T> b) {
    return a.x = b.x && a.y == b.y ;
}
template <typename T>
inline bool operator!=(Vector2<T> a, Vector2<T> b) {
    return !(a==b);
}




typedef Vector3<Real> Vector3f;
typedef Vector2<Real> Vector2f;
#endif // VECTOR3_H_INCLUDED
