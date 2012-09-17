
#ifndef STATEPLAY_HPP
#define STATEPLAY_HPP

#include "State.hpp"
#include "Text.hpp"
#include "InputString.hpp"
#include "Button.hpp"

class StatePlay : public State {
GAMESTATE
private:
	Sprite* inputbox;
	Text* inputtext;
	InputString inputstring;
	
	Button* goback;
	Button* quit;
public:
	StatePlay(ArgsBase* args);
	
	void update();
	void render();
private:
	void handleInput(const observer::Event& event, bool& stop);
	void handleEnter(const observer::Event& event, bool& stop);
	void handleGoBack(const observer::Event& event, bool& stop);
};

#endif
