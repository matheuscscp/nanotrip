
#ifndef STATEINSTRUCTIONS_HPP
#define STATEINSTRUCTIONS_HPP

#include "State.hpp"
#include "Button.hpp"

class StateInstructions : public State {
GAMESTATE
private:
	Button* goback;
	Button* quit;
public:
	StateInstructions(ArgsBase* args);
	
	void render();
private:
	void handleGoBack(const observer::Event& event, bool& stop);
};

#endif
