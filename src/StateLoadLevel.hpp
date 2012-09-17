
#ifndef STATELOADLEVEL_HPP
#define STATELOADLEVEL_HPP

#include "State.hpp"
#include "Text.hpp"
#include "Button.hpp"

class StateLoadLevel : public State {
GAMESTATE
private:
	Sprite* inputbox;
	Text* input;
	
	Button* goback;
	Button* quit;
public:
	StateLoadLevel(ArgsBase* args);
	
	void update();
	void render();
private:
	void handleGoBack(const observer::Event& event, bool& stop);
};

#endif
