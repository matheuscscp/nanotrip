
#ifndef STATEPLAY_HPP
#define STATEPLAY_HPP

#include "State.hpp"
#include "Text.hpp"
#include "InputString.hpp"
#include "Button.hpp"

class StatePlay : public State {
GAMESTATE
private:
	Sprite* bg;
	
	Sprite* inputbox;
	Text* inputtext;
	InputString inputstring;
	Text* warning;
	bool warning_hidden;
	
	Button* goback;
	Button* play;
public:
	StatePlay(ArgsBase* args);
	~StatePlay();
	
	void update();
	void render();
private:
	void handleInput(const observer::Event& event, bool& stop);
	void handleEnter(const observer::Event& event, bool& stop);
	void handleGoBack(const observer::Event& event, bool& stop);
};

#endif
