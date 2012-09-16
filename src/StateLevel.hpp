
#ifndef STATELEVEL_HPP
#define STATELEVEL_HPP

#include "State.hpp"

class StateLevel : public State {
GAMESTATE_DEC(StateLevel)
protected:
	Configuration raw;
	
	Particle* avatar;
	std::list<Particle*> particles;
	
	Sprite* positive_sprite;
	Sprite* negative_sprite;
public:
	StateLevel(ArgsBase* args);
protected:
	void load(const std::string& path);
	void reload();
	
	void assemble();
	void assembleAvatar(const Configuration& conf);
	void assembleParticle(Particle* particle, const Configuration& conf);
	void assembleInteractions();
};

#endif
