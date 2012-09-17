
#ifndef STATELEVEL_HPP
#define STATELEVEL_HPP

#include "State.hpp"
#include "Level.hpp"

class StateLevel : public State {
GAMESTATE
public:
	class Args : public ArgsBase {
	public:
		std::string levelname;
		Args(const std::string& levelname);
	};
protected:
	Level level;
	
	Sprite* positive_sprite;
	Sprite* negative_sprite;
public:
	StateLevel(ArgsBase* args);
	
	void update();
	void render();
};

#endif
