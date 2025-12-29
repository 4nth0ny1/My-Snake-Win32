#define TIMER_INTERNAL_OK 1
#include "timer.h"
#include "internal/timer_internal.h"

#include <windows.h>
#include <stdint.h>
#include <stdlib.h>

static void timer_init_internal(Win32Timer* timer) {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    timer->performanceFrequency = (int64_t)frequency.QuadPart;

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    timer->lastCounter = (int64_t)counter.QuadPart;
}

Win32Timer* timer_create(void) {
    Win32Timer* t = (Win32Timer*)malloc(sizeof(Win32Timer));
    if (!t) {
        return NULL;
    }

    timer_init_internal(t);
    return t;
}

void timer_destroy(Win32Timer* t) {
    if (t) {
        free(t);
    }
}

float timer_tick_seconds(Win32Timer* timer) {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    int64_t now = (int64_t)counter.QuadPart;
    int64_t delta = now - timer->lastCounter;
    timer->lastCounter = now;

    double seconds = 0.0;
    if (timer->performanceFrequency > 0) {
        seconds = (double)delta / (double)timer->performanceFrequency;
    }

    if (seconds < 0.0) seconds = 0.0;
    if (seconds > 0.05) seconds = 0.05;

    return (float)seconds;
}
