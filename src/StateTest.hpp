
#ifndef STATETEST_HPP
#define STATETEST_HPP

#include "State.hpp"

class StateTest : public State {
// for the game state ID
GAMESTATE
public:
	StateTest(ArgsBase* args);
	~StateTest();
	
	void input();
	void update();
	void render();
};

#endif
