#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "timer.h"
#include "win32_platform.h"

static Backbuffer* g_backbuffer = NULL;
static bool g_running = false;

static LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static void do_frame_work(HWND hwnd, float dt_seconds);

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR commandLine, _In_ int showCode) {
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

    RECT rc;
    GetClientRect(hwnd, &rc);
    int client_w = rc.right - rc.left;
    int client_h = rc.bottom - rc.top;
    
    g_backbuffer = backbuffer_create(client_w, client_h);
    if (!g_backbuffer) {
        MessageBoxA(0, "backbuffer_create failed", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }


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
    backbuffer_destroy(g_backbuffer);
    g_backbuffer = NULL;
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

    RECT rc;
    GetClientRect(hwnd, &rc);

    int client_w = rc.right - rc.left;
    int client_h = rc.bottom - rc.top;

    backbuffer_resize(g_backbuffer, client_w, client_h);

    uint32_t clear_color = 0xFF202040;
    backbuffer_clear(g_backbuffer, clear_color);

    HDC dc = GetDC(hwnd);
    backbuffer_present(dc, g_backbuffer, client_w, client_h);
    ReleaseDC(hwnd, dc);

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
