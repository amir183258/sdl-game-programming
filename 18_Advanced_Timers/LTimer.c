#include "LTimer.h"

void initTimer(LTimer *timer) {
	timer->mStartTicks = 0;
	timer->mPausedTicks = 0;

	timer->mPaused = 0;
	timer->mStarted = 0;
}

void startTimer(LTimer *timer) {
	timer->mStarted = 1;

	timer->mPaused = 0;

	timer->mStartTicks = SDL_GetTicks();
	timer->mPausedTicks = 0;
}

void stopTimer(LTimer *timer) {
	timer->mStarted = 0;
	timer->mPaused = 0;

	timer->mStartTicks = 0;
	timer->mPausedTicks = 0;
}

void pauseTimer(LTimer *timer) {
	if (timer->mStarted && !timer->mPaused) {
		timer->mPaused = 1;

		timer->mPausedTicks = SDL_GetTicks() - timer->mStartTicks;
		timer->mStartTicks = 0;
	}
}

void unpauseTimer(LTimer *timer) {
	if (timer->mStarted && timer->mPaused) {
		timer->mPaused = 0;

		timer->mStartTicks = SDL_GetTicks() - timer->mPausedTicks;

		timer->mPausedTicks = 0;
	}
}

Uint32 getTicksTimer(LTimer *timer) {
	Uint32 time = 0;

	if (timer->mStarted) {
		if (timer->mPaused)
			time = timer->mPausedTicks;
		else
			time = SDL_GetTicks() - timer->mStartTicks;
	}

	return time;
}

int isStartedTimer(LTimer *timer) {
	return timer->mStarted;
}

int isPausedTimer(LTimer *timer) {
	return timer->mPaused && timer->mStarted;
}
