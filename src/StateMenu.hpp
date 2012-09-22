
#ifndef STATEMENU_HPP
#define STATEMENU_HPP

#include "State.hpp"
#include "Button.hpp"

class StateMenu : public State {
GAMESTATE
private:
	Sprite* bg;
	
	Button* resume;
	Button* mainmenu;
public:
	StateMenu(ArgsBase* args);
	~StateMenu();
	
	void update();
	void render();
private:
	void handleKeyDown(const observer::Event& event, bool& stop);
	void handleResume(const observer::Event& event, bool& stop);
	void handleMainMenu(const observer::Event& event, bool& stop);
};

#endif
