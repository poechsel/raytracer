#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "windows.h"

/* fonctions permettant d'avoir acc�s au temps de fa�on pr�cise gr�ce � l'api
*  windows
*/
ULARGE_INTEGER getTime();
double getTimeElapsed(ULARGE_INTEGER tb);

#endif // BENCHMARK_H
