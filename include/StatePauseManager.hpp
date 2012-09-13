#ifndef STATEPAUSEMANAGER_HPP
#define STATEPAUSEMANAGER_HPP

#include "State.hpp"

class StatePauseManager : public State {
GAMESTATE
public:
	StatePauseManager(ArgsBase* args);
private:
	void handleKeyDown(const observer::Event& event, bool& stop);
};

#endif
