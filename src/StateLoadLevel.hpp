
#ifndef STATELOADLEVEL_HPP
#define STATELOADLEVEL_HPP

#include "State.hpp"
#include "Text.hpp"
#include "InputString.hpp"
#include "Button.hpp"

class StateLoadLevel : public State {
GAMESTATE
private:
	Sprite* inputbox;
	Text* inputtext;
	InputString inputstring;
	Text* warning;
	bool warning_hidden;
	
	Button* goback;
	Button* quit;
public:
	StateLoadLevel(ArgsBase* args);
	
	void update();
	void render();
private:
	void handleInput(const observer::Event& event, bool& stop);
	void handleEnter(const observer::Event& event, bool& stop);
	void handleGoBack(const observer::Event& event, bool& stop);
};

#endif
