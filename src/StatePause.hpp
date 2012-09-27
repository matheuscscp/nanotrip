
#ifndef STATEPAUSE_HPP
#define STATEPAUSE_HPP

#include "State.hpp"
#include "Button.hpp"

class StatePause : public State {
GAMESTATE
private:
	Sprite* bg;
	
	Button* retry;
	Button* menu;
	Button* resume;
public:
	StatePause(ArgsBase* args);
	~StatePause();
	
	void update();
	void render();
private:
	void handleKeyDown(const observer::Event& event, bool& stop);
	void handleRetry(const observer::Event& event, bool& stop);
	void handleMenu(const observer::Event& event, bool& stop);
	void handleResume(const observer::Event& event, bool& stop);
};

#endif
