
#ifndef STATEINSTRUCTIONS_HPP
#define STATEINSTRUCTIONS_HPP

#include "State.hpp"
#include "Button.hpp"

class StateInstructions : public State {
GAMESTATE
private:
	Sprite* bg;
	
	Button* back;
public:
	StateInstructions(ArgsBase* args);
	~StateInstructions();
	
	void update();
	void render();
private:
	void handleGoBack(const observer::Event& event, bool& stop);
};

#endif
