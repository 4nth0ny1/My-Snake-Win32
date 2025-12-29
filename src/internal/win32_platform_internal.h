#ifndef WIN32_PLATFORM_INTERNAL_H
#define WIN32_PLATFORM_INTERNAL_H

#ifndef WIN32_PLATFORM_INTERNAL_OK
#error "win32_platform_internal.h is private. Include win32_platform.h instead."
#endif

#include <windows.h>
#include <stdint.h>

struct Backbuffer {
    int width;
    int height;
    int pitch;          /* bytes per row */
    void* memory;       /* BGRA pixels */
    BITMAPINFO bmi;     /* header for StretchDIBits */
};

#endif // WIN32_PLATFORM_INTERNAL_H
