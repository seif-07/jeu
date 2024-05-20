#include <time.h>
#include <sys/time.h>

time_t time(time_t *t) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (t != NULL) {
        *t = tv.tv_sec;
    }
    return tv.tv_sec;
}

