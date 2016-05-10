#include "material.h"

Material::Material(Color c, Real op, Real refl, Real ior):
    _ior(ior), _opacity(op), _refl(refl), _diffuse(c)
{
    //ctor
}

Material::~Material()
{
    //dtor
}
