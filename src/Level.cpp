#include "Level.hpp"

#include "common.hpp"

#include "Interaction.hpp"

using namespace common;
using namespace lalge;

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
	assembleAvatar(raw.getConfig("avatar"));
	
	// all particles
	list<Configuration> conf = raw.getConfigList("particle");
	for (list<Configuration>::iterator it = conf.begin(); it != conf.end(); ++it) {
		assembleParticle(*it);
	}
	
	assembleInteractions();
}

void Level::assembleAvatar(const Configuration& conf) {
	avatar = new Particle();
	avatar->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	avatar->speed = r2vec(conf.getReal("speedX"), conf.getReal("speedX"));
	avatar->setElasticity(conf.getReal("k"));
	avatar->setMass(conf.getReal("m"));
	avatar->charge = conf.getReal("q");
}

void Level::assembleParticle(const Configuration& conf) {
	Particle* particle = new Particle();
	particle->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	particle->setElasticity(conf.getReal("k"));
	particle->setMass(conf.getReal("m"));
	particle->charge = conf.getReal("q");
	particles.push_back(particle);
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
