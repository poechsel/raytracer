#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "windows.h"

/* fonctions permettant d'avoir accés au temps de façon précise grâce à l'api
*  windows
*/
ULARGE_INTEGER getTime();
double getTimeElapsed(ULARGE_INTEGER tb);

#endif // BENCHMARK_H
