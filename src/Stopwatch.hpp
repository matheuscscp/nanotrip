#ifndef TIMER_HPP
#define TIMER_HPP

#include "observer.hpp"

class Timer {
private:
	int initialtime;
	int pausetime;
	bool paused;
public:
	Timer();
	~Timer();
	
	void start();
	void pause();
	void resume();
	
	int time() const;
	bool ispaused() const;
private:
	void handleObsStackPush(const observer::Event& event, bool& stop);
	void handleObsStackPop(const observer::Event& event, bool& stop);
};

#endif
