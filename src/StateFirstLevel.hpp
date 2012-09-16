
#ifndef STATEFIRSTLEVEL_HPP
#define STATEFIRSTLEVEL_HPP

#include "State.hpp"
#include "Level.hpp"

class StateFirstLevel : public State {
GAMESTATE
private:
	Level level;
	
	Sprite* positive_sprite;
	Sprite* negative_sprite;
public:
	StateFirstLevel(ArgsBase* args);
	
	void update();
	void render();
};

#endif
