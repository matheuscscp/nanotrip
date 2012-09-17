
#ifndef STATEDIRECTIONS_HPP
#define STATEDIRECTIONS_HPP

#include "State.hpp"
#include "Button.hpp"

class StateDirections : public State {
GAMESTATE
private:
	Button* goback;
	Button* quit;
public:
	StateDirections(ArgsBase* args);
private:
	void handleGoBack(const observer::Event& event, bool& stop);
};

#endif
