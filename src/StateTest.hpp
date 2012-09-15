
#ifndef STATETEST_HPP
#define STATETEST_HPP

#include "State.hpp"

class StateTest : public State {
GAMESTATE
public:
	StateTest(ArgsBase* args);
	
	void update();
	void render();
};

#endif
