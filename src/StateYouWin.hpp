
#ifndef STATEYOUWIN_HPP
#define STATEYOUWIN_HPP

#include "State.hpp"
#include "Button.hpp"
#include "Text.hpp"

class StateYouWin : public State {
GAMESTATE
private:
	Sprite* bg;
	
	Text* text_you_win;
	Text* text_points;
	
	Button* tryagain;
	Button* cont;
public:
	StateYouWin(ArgsBase* args);
	~StateYouWin();
	
	void update();
	void render();
private:
	void handleTryAgain(const observer::Event& event, bool& stop);
	void handleContinue(const observer::Event& event, bool& stop);
};

#endif
