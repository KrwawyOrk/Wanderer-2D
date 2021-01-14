#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include "SDL.h"

class EventReceiver
{
public:
	EventReceiver(SDL_Event* e );
	virtual ~EventReceiver(void);

	virtual void InputEvents( void ) = 0;

private:
	SDL_Event* e;
};

#endif