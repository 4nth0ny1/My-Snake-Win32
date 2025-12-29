#define WIN32_PLATFORM_INTERNAL_OK 1
#include "win32_platform.h"
#include "internal/win32_platform_internal.h"

#include <windows.h>
#include <stdint.h>
#include <stdlib.h>

static void backbuffer_free_pixels(Backbuffer* b) {
    if (b->memory) {
        VirtualFree(b->memory, 0, MEM_RELEASE);
        b->memory = 0;
    }
    b->width = 0;
    b->height = 0;
    b->pitch = 0;
}

Backbuffer* backbuffer_create(int width, int height) {
    Backbuffer* b = (Backbuffer*)malloc(sizeof(Backbuffer));
    if (!b) {
        return NULL;
    }

    b->width = 0;
    b->height = 0;
    b->pitch = 0;
    b->memory = 0;
    ZeroMemory(&b->bmi, sizeof(b->bmi));

    if (!backbuffer_resize(b, width, height)) {
        backbuffer_destroy(b);
        return NULL;
    }

    return b;
}

void backbuffer_destroy(Backbuffer* b) {
    if (!b) {
        return;
    }

    backbuffer_free_pixels(b);
    free(b);
}

int backbuffer_resize(Backbuffer* b, int new_w, int new_h) {
    if (!b) {
        return 0;
    }

    if (new_w <= 0 || new_h <= 0) {
        backbuffer_free_pixels(b);
        return 0;
    }

    if (b->memory && b->width == new_w && b->height == new_h) {
        return 1;
    }

    backbuffer_free_pixels(b);

    b->width = new_w;
    b->height = new_h;

    {
        int bytes_per_pixel = 4;
        size_t total_bytes;

        b->pitch = b->width * bytes_per_pixel;
        total_bytes = (size_t)b->pitch * (size_t)b->height;

        b->memory = VirtualAlloc(0, total_bytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (!b->memory) {
            backbuffer_free_pixels(b);
            return 0;
        }
    }

    b->bmi.bmiHeader.biSize = sizeof(b->bmi.bmiHeader);
    b->bmi.bmiHeader.biWidth = b->width;
    b->bmi.bmiHeader.biHeight = -b->height; /* top-down */
    b->bmi.bmiHeader.biPlanes = 1;
    b->bmi.bmiHeader.biBitCount = 32;
    b->bmi.bmiHeader.biCompression = BI_RGB;
    b->bmi.bmiHeader.biSizeImage = 0;

    return 1;
}

void backbuffer_clear(Backbuffer* b, uint32_t color_bgra) {
    if (!b || !b->memory) {
        return;
    }

    {
        uint8_t* row = (uint8_t*)b->memory;
        int y;

        for (y = 0; y < b->height; y += 1) {
            uint32_t* pixel = (uint32_t*)row;
            int x;

            for (x = 0; x < b->width; x += 1) {
                pixel[x] = color_bgra;
            }

            row += b->pitch;
        }
    }
}

void backbuffer_present(HDC dc, const Backbuffer* b, int dest_w, int dest_h) {
    if (!b || !b->memory) {
        return;
    }

    StretchDIBits(
        dc,
        0, 0, dest_w, dest_h,
        0, 0, b->width, b->height,
        b->memory,
        &b->bmi,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}
