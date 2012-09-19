#include "Level.hpp"

#include "common.hpp"

#include "Interaction.hpp"

using namespace common;
using namespace lalge;

using std::string;
using std::list;

Level::Level() : avatar(0) {
	// all sprites
	sprite_avatar = new Animation("img/avatar.png", altas parada);
	sprite_negative = new Sprite("img/negative.png");
	sprite_neutral = new Sprite("img/neutral.png");
	sprite_positive = new Sprite("img/positive.png");
}

Level::~Level() {
	dropAll();
	
	// all sprites
	delete sprite_avatar;
	delete sprite_negative;
	delete sprite_neutral;
	delete sprite_positive;
}

void Level::load(const string& path) {
	raw.clear();
	raw.readTxt(RootPath::get(path));
	reload();
}

void Level::reload() {
	dropAll();
	assemble();
}

void Level::update() {
	// all interactions
	for (list<Interaction*>::iterator it = interactions.begin(); it != interactions.end(); ++it) {
		(*it)->interact();
	}
	
	// the avatar
	avatar->update();
	
	// all particles
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		(*it)->update();
	}
	
	// all sprites
	sprite_avatar->update();
}

void Level::render() {
	// all interactions
	for (list<Interaction*>::iterator it = interactions.begin(); it != interactions.end(); ++it) {
		(*it)->interact();
	}
	
	// the avatar
	avatar->update();
	
	// all particles
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		(*it)->update();
	}
}

void Level::assemble() {
	assembleAvatar(raw.getConfig("avatar"));
	
	// all particles
	list<Configuration> conf = raw.getConfigList("particle");
	for (list<Configuration>::iterator it1 = conf.begin(); it1 != conf.end(); ++it1) {
		Particle* particle = assembleParticle(*it1);
		// avatar interactions
		interactions.push_back(Interaction(particle, avatar, (Interaction::callback)&Particle::manageParticleCollision));
		interactions.push_back(Interaction(particle, avatar, (Interaction::callback)&Particle::addParticleFieldForces, true));
		// mutual interactions
		for (list<Particle*>::iterator it2 = particles.begin(); it2 != particles.end(); ++it2) {
			interactions.push_back(Interaction(particle, *it2, (Interaction::callback)&Particle::manageParticleCollision));
			interactions.push_back(Interaction(particle, *it2, (Interaction::callback)&Particle::addParticleFieldForces, true));
		}
		particles.push_back(particle);
		// setting sprite
		if (avatar->charge < 0)
			avatar->sprite = sprite_negative;
		else if (avatar->charge == 0)
			avatar->sprite = sprite_neutral;
		else
			avatar->sprite = sprite_positive;
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
	avatar->sprite = sprite_avatar;
}

Particle* Level::assembleParticle(const Configuration& conf) {
	Particle* particle = new Particle();
	particle->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	particle->setElasticity(conf.getReal("k"));
	particle->setMass(conf.getReal("m"));
	particle->charge = conf.getReal("q");
	return particle;
}

void Level::dropAll() {
	// the avatar
	delete avatar;
	
	// all particles
	while (particles.size()) {
		delete particles.back();
		particles.pop_back();
	}
	
	// all interactions
	while (interactions.size()) {
		delete interactions.back();
		interactions.pop_back();
	}
}
