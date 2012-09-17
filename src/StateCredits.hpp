
#ifndef STATECREDITS_HPP
#define STATECREDITS_HPP

#include "State.hpp"
#include "Button.hpp"

class StateCredits : public State {
GAMESTATE
private:
	Button* goback;
	Button* quit;
public:
	StateCredits(ArgsBase* args);
private:
	void handleGoBack(const observer::Event& event, bool& stop);
};

#endif
