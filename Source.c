#include <windows.h>

#include <stdbool.h>

static bool g_running = false;

static LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		
		case WM_CLOSE: 
			g_running = false;
		break;
		case WM_DESTROY:
			g_running = false;
			PostQuitMessage(0);
			return;
		break;


		default: 
			return DefWindowProcA(hwnd, msg, wParam, lParam);
		break;
	}
}


int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int showCode) {
	(void)prevInstance;
	(void)commandLine;
	(void)showCode;
	
	WNDCLASSEXA wc = {0}; // https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassa

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

	RegisterClassExA(&wc); // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassexa

	HWND hwnd = CreateWindowExA(			// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
		WS_EX_TRANSPARENT,					// dwExStyle
		wc.lpszClassName,					// lpClassName
		"My Biggest Snake",					// lpWindowName, controls the name on the window bar
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// dwStyle
		CW_USEDEFAULT, CW_USEDEFAULT,		// X, Y
		900, 700,							// nWidth, nHeight, 
		0, 0, instance, 0					// hWndParent, hMenu, hInstance, lpParam
	);


	g_running = true;
	while (g_running) {
		MSG msg;
		if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {			// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagea
			if (msg.message == WM_QUIT) return false;
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	return 0;
}