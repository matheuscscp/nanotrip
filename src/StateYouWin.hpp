
#ifndef STATEYOUWIN_HPP
#define STATEYOUWIN_HPP

#include "State.hpp"
#include "Button.hpp"
#include "Text.hpp"

class StateYouWin : public State {
GAMESTATE
private:
	Sprite* bg;
	
	Sprite* shadow;
	Sprite* eatles;
	
	Text* text_score;
	Text* text_points;
	
	Button* retry;
	Button* menu;
	Button* next;
public:
	StateYouWin(ArgsBase* args);
	~StateYouWin();
	
	void update();
	void render();
private:
	void handleRetry(const observer::Event& event, bool& stop);
	void handleMenu(const observer::Event& event, bool& stop);
	void handleNext(const observer::Event& event, bool& stop);
};

#endif
