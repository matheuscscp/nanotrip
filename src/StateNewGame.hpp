
#ifndef STATENEWGAME_HPP
#define STATENEWGAME_HPP

#include "State.hpp"
#include "Button.hpp"

class StateNewGame : public State {
GAMESTATE
private:
	Sprite* bg;
	Sprite* mainmenu;
	
	Button* playstory;
	Button* loadlevel;
	Button* makelevel;
	Button* back;
public:
	StateNewGame(ArgsBase* args);
	~StateNewGame();
	
	void update();
	void render();
private:
	void handlePlayStory(const observer::Event& event, bool& stop);
	void handleLoadLevel(const observer::Event& event, bool& stop);
	void handleMakeLevel(const observer::Event& event, bool& stop);
	void handleGoBack(const observer::Event& event, bool& stop);
};

#endif
