
#ifndef STATECREDITS_HPP
#define STATECREDITS_HPP

#include "State.hpp"
#include "Button.hpp"

class StateCredits : public State {
GAMESTATE
private:
	Sprite* bg;
	
	Button* goback;
public:
	StateCredits(ArgsBase* args);
	~StateCredits();
	
	void update();
	void render();
private:
	void handleGoBack(const observer::Event& event, bool& stop);
};

#endif
