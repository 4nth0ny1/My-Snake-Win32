#ifndef TIMER_INTERNAL_H
#define TIMER_INTERNAL_H

#ifndef TIMER_INTERNAL_OK
#error "timer_internal.h is private. Include timer.h instead."
#endif

#include <stdint.h>

struct Win32Timer {
    int64_t performanceFrequency;
    int64_t lastCounter;
};

#endif // TIMER_INTERNAL_H
