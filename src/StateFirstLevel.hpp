
#ifndef STATEFIRSTLEVEL_HPP
#define STATEFIRSTLEVEL_HPP

#include "State.hpp"

class StateFirstLevel : public State {
GAMESTATE
private:
	Sprite* positive_sprite;
	Sprite* negative_sprite;
public:
	StateFirstLevel(ArgsBase* args);
	
	void update();
	void render();
};

#endif
