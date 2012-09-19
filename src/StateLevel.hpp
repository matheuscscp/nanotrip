
#ifndef STATELEVEL_HPP
#define STATELEVEL_HPP

#include "configfile.hpp"

#include "State.hpp"
#include "Particle.hpp"

class StateLevel : public State {
GAMESTATE
public:
	class Args : public ArgsBase {
	public:
		std::string levelname;
		Args(const std::string& levelname);
	};
protected:
	Sprite* bg;
	Sprite* sprite_avatar;
	Sprite* sprite_negative;
	Sprite* sprite_neutral;
	Sprite* sprite_positive;
	
	Configuration raw;
	
	std::list<Interaction> interactions;
	
	Particle* avatar;
	std::list<Particle*> particles;
public:
	StateLevel(ArgsBase* args);
	~StateLevel();
	
	void update();
	void render();
protected:
	void reload();
	void assemble();
	void assembleAvatar();
	Particle* assembleParticle(const Configuration& conf);
	void clear();
};

#endif
