#ifndef TIMER_H
#define TIMER_H

#include "SDL.h"

class Timer
{
public:
	Timer()	: m_startTime( SDL_GetTicks() / 1000 ) {}
	int GetElapsedTime( void ) const { return SDL_GetTicks() / 1000 - m_startTime; }

private:
	int m_startTime;
};

#endif