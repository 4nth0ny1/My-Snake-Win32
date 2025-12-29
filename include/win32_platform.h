#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <stdint.h>

	typedef struct Backbuffer Backbuffer;

	/* Allocate + initialize. Returns NULL on failure. */
	Backbuffer* backbuffer_create(int width, int height);

	/* Free. Safe to pass NULL. */
	void backbuffer_destroy(Backbuffer* b);

	/* Resize. Returns 1 on success, 0 on failure. */
	int backbuffer_resize(Backbuffer* b, int new_w, int new_h);

	/* Clear to a BGRA 32-bit color. */
	void backbuffer_clear(Backbuffer* b, uint32_t color_bgra);

	/* Present to the window. */
	void backbuffer_present(HDC dc, const Backbuffer* b, int dest_w, int dest_h);

#ifdef __cplusplus
}
#endif

#endif // WIN32_PLATFORM_H
