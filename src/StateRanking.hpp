
#ifndef STATERANKING_HPP
#define STATERANKING_HPP

#include "State.hpp"
#include "Button.hpp"

class StateRanking : public State {
GAMESTATE
private:
	Button* goback;
	Button* quit;
public:
	StateRanking(ArgsBase* args);
	
	void render();
private:
	void handleGoBack(const observer::Event& event, bool& stop);
};

#endif
