
#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <list>
#include <string>

#include "configfile.hpp"

#include "Particle.hpp"

class Level {
private:
	Configuration raw;
public:
	Particle* avatar;
	std::list<Particle*> particles;
	std::list<Interaction*> interactions;
	
	Sprite* sprite_avatar;
	Sprite* sprite_negative;
	Sprite* sprite_neutral;
	Sprite* sprite_positive;
	
	Level();
	~Level();
	
	void load(const std::string& path);
	void reload();
	
	void update();
	void render();
private:
	void assemble();
	void assembleAvatar(const Configuration& conf);
	Particle* assembleParticle(const Configuration& conf);
	void dropAll();
};

#endif
