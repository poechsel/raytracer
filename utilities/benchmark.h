#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "windows.h"

ULARGE_INTEGER getTime();
double getTimeElapsed(ULARGE_INTEGER tb);

#endif // BENCHMARK_H
