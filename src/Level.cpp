#include "Level.hpp"

#include "common.hpp"

#include "Interaction.hpp"

using namespace common;

using std::string;
using std::list;

void Level::load(const string& path) {
	raw.clear();
	raw.readTxt(RootPath::get(path));
	reload();
}

void Level::reload() {
	// the avatar
	if (avatar)
		delete avatar;
	
	// all particles
	while (particles.size()) {
		delete particles.back();
		particles.pop_back();
	}
	
	assemble();
}

void Level::assemble() {
	
	assembleInteractions();
}

void Level::assembleAvatar(const Configuration& avatar) {
	
}

void Level::assembleParticle(const Configuration& particle) {
	
}

void Level::assembleInteractions() {
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		// avatar interactions
		Interaction::insert(avatar, *it, (Interaction::callback)&Particle::manageParticleCollision);
		Interaction::insert(avatar, *it, (Interaction::callback)&Particle::addParticleFieldForces, true);
		
		// mutual interactions
		list<Particle*>::iterator it2 = it;
		++it2;
		for (; it2 != particles.end(); ++it2) {
			Interaction::insert(*it, *it2, (Interaction::callback)&Particle::manageParticleCollision);
			Interaction::insert(*it, *it2, (Interaction::callback)&Particle::addParticleFieldForces, true);
		}
	}
}
