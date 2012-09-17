
#ifndef STATELOADLEVEL_HPP
#define STATELOADLEVEL_HPP

#include "State.hpp"
#include "Button.hpp"

class StateLoadLevel : public State {
GAMESTATE
private:
	Button* history;
	Button* loadlevel;
	Button* makelevel;
	Button* goback;
public:
	StateLoadLevel(ArgsBase* args);
	
	void update();
	void render();
private:
	void handleHistory(const observer::Event& event, bool& stop);
	void handleLoadLevel(const observer::Event& event, bool& stop);
	void handleMakeLevel(const observer::Event& event, bool& stop);
	void handleGoBack(const observer::Event& event, bool& stop);
};

#endif
