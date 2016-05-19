#ifndef COLOR_H
#define COLOR_H

#include "../includes.h"

/* Classe presque identiques à celle de Vector3, elle implémente les couleurs de
*  fàçon à pouvoir les manipuler aisément
*/
class Color {
    public:
        float r;
        float g;
        float b;
        float a;

        Color  (float r = 0, float g = 0, float b = 0, float a = 1) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        virtual ~Color  (){}

        Color & operator/=(float& rhs) {
            r /= rhs;
            g /= rhs;
            b /= rhs;
            a /= rhs;
            return *this;
        }
        Color & operator*=(float& rhs) {
            r *= rhs;
            g *= rhs;
            b *= rhs;
            a *= rhs;
            return *this;
        }
    private:
};


inline Color  operator+(Color  a, Color  b) {
    return Color (a.r+b.r, a.g+b.g, a.b+b.b, a.a + b.a);
}

inline Color  operator-(Color  a, Color  b) {
    return Color (a.r-b.r, a.g-b.g, a.b-b.b, a.a-b.a);
}

inline Color  operator*(Color  a, float k) {
    return Color (a.r*k, a.g*k, a.b*k, a.a*k);
}

inline Color  operator*(float k, Color  a) {
    return Color (a.r*k, a.g*k, a.b*k, a.a*k);
}
inline Color  operator/(Color  a, float k) {
    return Color (a.r/k, a.g/k, a.b/k, a.a/k);
}
inline Color  operator/(float k, Color a) {
    return Color (a.r/k, a.g/k, a.b/k, a.a/k);
}
inline bool operator==(Color  a, Color  b) {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}
inline bool operator!=(Color  a, Color  b) {
    return !(a==b);
}
#endif // COLOR_H
