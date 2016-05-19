#ifndef MATTRANSFORMATIONS_H
#define MATTRANSFORMATIONS_H

#include "../includes.h"

/* ce fichier implémente des transformations sur les matrices classiques mais
*  n'est pas utilisé
*/


/* fov is the whole angle, not the half
            /.\
           /fov\
          /     \
*/
Matrix4f perspective(Real width, Real height,
                     Real near, Real far,
                     Real fov);
Matrix4f orthographic(Real left, Real right,
                      Real bottom, Real top,
                      Real near, Real far) ;
#endif // MATTRANSFORMATIONS_H
