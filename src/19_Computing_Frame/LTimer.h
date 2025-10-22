#ifndef SDL_VERSION
#include <SDL.h>
#endif

struct LTimer {
	Uint32 mStartTicks;

	Uint32 mPausedTicks;

	int mPaused;
	int mStarted;
};
typedef struct LTimer LTimer;

void initTimer(LTimer *timer);
void startTimer(LTimer *timer);
void stopTimer(LTimer *timer);
void pauseTimer(LTimer *timer);
void unpauseTimer(LTimer *timer);
Uint32 getTicksTimer(LTimer *timer);
int isStartedTimer(LTimer *timer);
int isPausedTimer(LTimer *timer);

