#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

 
// Alias
typedef int64_t i64;




// Structs
typedef struct {
	i64 performanceFrequency;
	i64 lastCounter;
} Win32Timer;







// Global Variables
static bool g_running = false;







// Function Declarations
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int showCode);
static LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static void do_frame_work(HWND hwnd, float dt_seconds);
static void timer_init(Win32Timer* t);
static float timer_update_and_get_seconds(Win32Timer* t);




// Function Implementations
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int showCode) {
	(void)prevInstance;
	(void)commandLine;
	(void)showCode;
	
	WNDCLASSEXA wc = {0}; 

	wc.cbSize			= sizeof(WNDCLASSEXA);
	wc.style			= CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc		= Win32WindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= instance;
	wc.hIcon			= NULL;
	wc.hCursor			= NULL;
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= "My Snake";
	wc.lpszClassName	= "My Big Snake";
	wc.hIconSm			= NULL;

	RegisterClassExA(&wc); 

	HWND hwnd = CreateWindowExA(			
		WS_EX_TRANSPARENT,					// dwExStyle
		wc.lpszClassName,					// lpClassName
		"My Biggest Snake",					// lpWindowName, controls the name on the window bar
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// dwStyle
		CW_USEDEFAULT, CW_USEDEFAULT,		// X, Y
		900, 700,							// nWidth, nHeight, 
		0, 0, instance, 0					// hWndParent, hMenu, hInstance, lpParam
	);

	// Timing Init
	Win32Timer timer;
	timer_init(&timer);
	
	// Main Loop
	g_running = true;
	while (g_running) {
		MSG msg;
		if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {			
			if (msg.message == WM_QUIT) return false;
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		float dt_seconds = timer_update_and_get_seconds(&timer);

		do_frame_work(hwnd, dt_seconds);
	
		Sleep(1);
	}
	return 0;
}

static LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { 
	switch (msg) {

		case WM_CLOSE:
			g_running = false;
			return 0;
		break;
		case WM_DESTROY:
			g_running = false;
			PostQuitMessage(0);
			return 0;
		break;


		default:
			return DefWindowProcA(hwnd, msg, wParam, lParam);
		break;
	}
}






// Rendering 
static void do_frame_work(HWND hwnd, float dt_seconds) {
	static float title_timer = 0.0f;
	title_timer += dt_seconds;

	if (title_timer >= 0.25f) {
		title_timer = 0.0f;

		float fps = 0.0f;
		if (dt_seconds > 0.0f) fps = 1.0f / dt_seconds;

		char buffer[256];

		snprintf(buffer, sizeof(buffer), "Snake - dt: %.4f  fps: %.1f", dt_seconds, fps);
		SetWindowTextA(hwnd, buffer);
	}
}






// Timing functions
static void timer_init(Win32Timer* timer) {
	LARGE_INTEGER freqency;
	QueryPerformanceFrequency(&freqency);
	timer->performanceFrequency = (int64_t)freqency.QuadPart;

	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	timer->lastCounter = (int64_t)counter.QuadPart;
}

static float timer_update_and_get_seconds(Win32Timer* timer) {
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	int64_t now = (int64_t)counter.QuadPart;
	int64_t delta = now - timer->lastCounter;
	timer->lastCounter = now;

	double seconds = 0.0;
	if (timer->performanceFrequency > 0) seconds = (double)delta / (double)timer->performanceFrequency;
	if (seconds < 0.0) seconds = 0.0;
	if (seconds > 0.05) seconds = 0.05;

	return (float)seconds;
}