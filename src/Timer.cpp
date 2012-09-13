#include "SDL.h"

#include "mod/observer.hpp"

#include "Timer.hpp"

Timer::Timer() : initialtime(-1), paused(false) {
	observer::Stack::connect(this, &Timer::handleObsStackPush, &Timer::handleObsStackPop);
}

Timer::~Timer() {
	observer::Stack::disconnect(this);
}

void Timer::start()
{
	initialtime = SDL_GetTicks();
	paused = false;
}

void Timer::pause()
{
	if( ( !paused ) && ( initialtime != -1 ) )
	{
		pausetime = SDL_GetTicks();
		paused = true;
	}
}

void Timer::resume()
{
	if( paused )
	{
		initialtime += ( SDL_GetTicks() - pausetime );
		paused = false;
	}
}

int Timer::time() const
{
	if( initialtime == -1 )
		return -1;
	else if( paused )
		return( pausetime - initialtime );
	
	return( SDL_GetTicks() - initialtime );
}

void Timer::handleObsStackPush(const observer::Event& event, bool& stop) { pause(); }
void Timer::handleObsStackPop(const observer::Event& event, bool& stop) { resume(); }
