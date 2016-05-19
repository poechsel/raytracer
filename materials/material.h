#ifndef MATERIAL_H
#define MATERIAL_H

#include "../utilities/color.h"


/*représente un matériel, provient surtout d'une vieille architecture du
* programme
*/
class Material
{
    public:
        Material(Color c, Real op = 1, Real refl = 0, Real ior = 1);
        Real getOpacity(){return _opacity;}
        Real getIor(){return _ior;}
        Real getReflexivity(){return _refl;}
        Color getDiffuse(){return _diffuse;}

        virtual ~Material();
    protected:
        Real _opacity;
        Real _ior;
        Real _refl;
        Color _diffuse;
    private:
};

#endif // MATERIAL_H
