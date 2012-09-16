
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
	
	void load(const std::string& path);
	void reload();
private:
	void assemble();
	void assembleAvatar(const Configuration& avatar);
	void assembleParticle(const Configuration& particle);
	void assembleInteractions();
};

#endif
