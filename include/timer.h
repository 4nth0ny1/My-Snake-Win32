#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct Win32Timer Win32Timer;

	/* Allocate + initialize a timer. Returns NULL on failure. */
	Win32Timer* timer_create(void);

	/* Free a timer created by timer_create. Safe to pass NULL. */
	void timer_destroy(Win32Timer* t);

	/* Measure elapsed time since last call. Returns seconds (clamped). */
	float timer_tick_seconds(Win32Timer* t);

#ifdef __cplusplus
}
#endif

#endif // TIMER_H
