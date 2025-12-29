#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "timer.h"


static bool g_running = false;

static LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static void do_frame_work(HWND hwnd, float dt_seconds);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int showCode) {
    (void)prevInstance;
    (void)commandLine;
    (void)showCode;

    WNDCLASSEXA wc = { 0 };

    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = Win32WindowProc;
    wc.hInstance = instance;
    wc.lpszMenuName = "My Snake";
    wc.lpszClassName = "My Big Snake";

    RegisterClassExA(&wc);

    HWND hwnd = CreateWindowExA(
        WS_EX_TRANSPARENT,
        wc.lpszClassName,
        "My Biggest Snake",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        900, 700,
        0, 0, instance, 0
    );

    // Timing init (opaque type, but we can still allocate it by value)
    Win32Timer* timer = timer_create();
    if (!timer) {
        MessageBoxA(0, "timer_create failed", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    g_running = true;
    while (g_running) {
        MSG msg;
        while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                g_running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        float dt_seconds = timer_tick_seconds(timer);
        do_frame_work(hwnd, dt_seconds);

        Sleep(1);
    }
    timer_destroy(timer);
    return 0;
}

static LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    (void)wParam;
    (void)lParam;

    switch (msg) {
        case WM_CLOSE:
            g_running = false;
            return 0;

        case WM_DESTROY:
            g_running = false;
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
}

static void do_frame_work(HWND hwnd, float dt_seconds) {
    static float title_timer = 0.0f;
    title_timer += dt_seconds;

    if (title_timer >= 0.25f) {
        title_timer = 0.0f;

        float fps = 0.0f;
        if (dt_seconds > 0.0f) {
            fps = 1.0f / dt_seconds;
        }

        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Snake - dt: %.4f  fps: %.1f", dt_seconds, fps);
        SetWindowTextA(hwnd, buffer);
    }
}
