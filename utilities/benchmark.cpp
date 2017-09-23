#include "benchmark.h"


#ifdef __WIN32
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
#else
ULARGE_INTEGER getTime(){
    return 0;
}
double getTimeElapsed(ULARGE_INTEGER tb){
    return 0;
}

#endif
