#ifndef BENCHMARK_H
#define BENCHMARK_H

#ifdef __WIN32
#include "windows.h"
#else
#include <cstdint>
#include <time.h>
typedef uint64_t ULARGE_INTEGER;
#endif

/* fonctions permettant d'avoir accés au temps de façon précise grâce à l'api
*  windows
*/
ULARGE_INTEGER getTime();
double getTimeElapsed(ULARGE_INTEGER tb);


#endif // BENCHMARK_H
