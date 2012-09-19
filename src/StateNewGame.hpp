
#ifndef STATENEWGAME_HPP
#define STATENEWGAME_HPP

#include "State.hpp"
#include "Button.hpp"

class StateNewGame : public State {
GAMESTATE
private:
	Sprite* bg;
	
	Button* play;
	Button* loadlevel;
	Button* makelevel;
	Button* goback;
public:
	StateNewGame(ArgsBase* args);
	~StateNewGame();
	
	void update();
	void render();
private:
	void handlePlay(const observer::Event& event, bool& stop);
	void handleLoadLevel(const observer::Event& event, bool& stop);
	void handleMakeLevel(const observer::Event& event, bool& stop);
	void handleGoBack(const observer::Event& event, bool& stop);
};

#endif
