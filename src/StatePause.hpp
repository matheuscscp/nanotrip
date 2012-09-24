
#ifndef STATEPAUSE_HPP
#define STATEPAUSE_HPP

#include "State.hpp"
#include "Button.hpp"
#include "Text.hpp"

class StatePause : public State {
GAMESTATE
private:
	Sprite* bg;
	
	Text* pause;
	
	Button* resume;
	Button* restart;
	Button* menu;
public:
	StatePause(ArgsBase* args);
	~StatePause();
	
	void update();
	void render();
private:
	void handleKeyDown(const observer::Event& event, bool& stop);
	void handleResume(const observer::Event& event, bool& stop);
	void handleRestart(const observer::Event& event, bool& stop);
	void handleMenu(const observer::Event& event, bool& stop);
};

#endif
