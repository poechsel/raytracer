#ifndef BENCHMARK_H
#define BENCHMARK_H

#ifdef __WIN32
#include "windows.h"
#else
#include <cstdint>
#include <time.h>
typedef uint64_t ULARGE_INTEGER;
#endif

/* fonctions permettant d'avoir acc�s au temps de fa�on pr�cise gr�ce � l'api
*  windows
*/
ULARGE_INTEGER getTime();
double getTimeElapsed(ULARGE_INTEGER tb);


#endif // BENCHMARK_H
