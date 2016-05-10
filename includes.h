#ifndef INCLUDES_H_INCLUDED
#define INCLUDES_H_INCLUDED

#include <vector>
#include <cmath>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
typedef double Real;
typedef unsigned int uint;


#include "maths/vector2.h"
#include "maths/vector3.h"

template <class T>
class Matrix4;
typedef Matrix4<Real> Matrix4f;


#include "maths/matrix4.h"
#include "maths/matTransformations.h"

#include "utilities/stringutilities.h"


#endif // INCLUDES_H_INCLUDED
