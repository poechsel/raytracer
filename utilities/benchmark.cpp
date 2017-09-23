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
    struct timespec request;
    clock_gettime(CLOCK_REALTIME, &request);
    return request;
}
double getTimeElapsed(ULARGE_INTEGER tb){
    ULARGE_INTEGER te = getTime();
    double accum = ( te.tv_sec - tb.tv_sec )
        + ( te.tv_nsec - tb.tv_nsec )
        / 1000000000.;
    return accum; 
}

#endif
