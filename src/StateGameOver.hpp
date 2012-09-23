
#ifndef STATEGAMEOVER_HPP
#define STATEGAMEOVER_HPP

#include "State.hpp"
#include "Button.hpp"

class StateGameOver : public State {
GAMESTATE
private:
	Sprite* bg;
	
	Button* tryagain;
	Button* mainmenu;
public:
	StateGameOver(ArgsBase* args);
	~StateGameOver();
	
	void update();
	void render();
private:
	void handleTryAgain(const observer::Event& event, bool& stop);
	void handleMainMenu(const observer::Event& event, bool& stop);
};

#endif
