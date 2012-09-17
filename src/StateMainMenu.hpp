
#ifndef STATEMAINMENU_HPP
#define STATEMAINMENU_HPP

#include "State.hpp"
#include "Button.hpp"

class StateMainMenu : public State {
GAMESTATE
private:
	Button* newgame;
	Button* ranking;
	Button* directions;
	Button* credits;
	Button* quit;
public:
	StateMainMenu(ArgsBase* args);
	
	void update();
	void render();
private:
	void handleNewGame(const observer::Event& event, bool& stop);
	void handleRanking(const observer::Event& event, bool& stop);
	void handleDirections(const observer::Event& event, bool& stop);
	void handleCredits(const observer::Event& event, bool& stop);
};

#endif
