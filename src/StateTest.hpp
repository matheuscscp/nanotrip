
#ifndef STATETEST_HPP
#define STATETEST_HPP

#include "State.hpp"

class StateTest : public State {
GAMESTATE_DEC(StateTest)
public:
	StateTest(ArgsBase* args);
	
	void update();
	void render();
};

#endif
