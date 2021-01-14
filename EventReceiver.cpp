#include "EventReceiver.h"


EventReceiver::EventReceiver( SDL_Event* e )
{
	this->e = e;
}


EventReceiver::~EventReceiver(void)
{
	this->e = NULL;
}
