
#ifndef STATEYOULOSE_HPP
#define STATEYOULOSE_HPP

#include "State.hpp"
#include "Button.hpp"

class StateYouLose : public State {
GAMESTATE
private:
	Sprite* bg;
	
	Sprite* shadow;
	Sprite* eatles;
	
	Button* retry;
	Button* menu;
public:
	StateYouLose(ArgsBase* args);
	~StateYouLose();
	
	void update();
	void render();
private:
	void handleRetry(const observer::Event& event, bool& stop);
	void handleMenu(const observer::Event& event, bool& stop);
};

#endif
