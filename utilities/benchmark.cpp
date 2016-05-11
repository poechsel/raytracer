#include "benchmark.h"


ULARGE_INTEGER getTime(){
    ULARGE_INTEGER tb;
    FILETIME ttmp = {0, 0};
    ::GetSystemTimeAsFileTime(&ttmp);
    tb.HighPart = ttmp.dwHighDateTime;
    tb.LowPart = ttmp.dwLowDateTime;
    return tb;
}
double getTimeElapsed(ULARGE_INTEGER tb){
    ULARGE_INTEGER te = getTime();
    return ((double)((te.QuadPart-tb.QuadPart)/10000))/1000.;
}