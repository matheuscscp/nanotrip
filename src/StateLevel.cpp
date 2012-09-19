#include "StateLevel.hpp"

#include "Animation.hpp"
#include "Circle.hpp"

using namespace common;
using namespace lalge;

using std::list;

GAMESTATE_DEF(StateLevel)

StateLevel::Args::Args(const std::string& levelname) : levelname(levelname) {}

StateLevel::StateLevel(ArgsBase* args) {
	// background
	bg = new Sprite("img/level/background.png");
	bg->render();
	bg->setAlpha(0.3f);
	
	// all sprites
	sprite_avatar = new Animation("img/level/avatar.png", 0, 7, 1, 16);
	sprite_negative = new Sprite("img/level/negative.png");
	sprite_neutral = new Sprite("img/level/neutral.png");
	sprite_positive = new Sprite("img/level/positive.png");
	
	// configuration file
	raw.readTxt(RootPath::get("level/" + ((Args*)args)->levelname + ".conf"));
	
	assemble();
	
	delete args;
}

StateLevel::~StateLevel() {
	clear();
	
	// all sprites
	delete bg;
	delete sprite_avatar;
	delete sprite_negative;
	delete sprite_neutral;
	delete sprite_positive;
}

void StateLevel::update() {
	// avatar animation
	sprite_avatar->update();
	
	// all interactions
	for (list<Interaction>::iterator it = interactions.begin(); it != interactions.end(); ++it) {
		it->interact();
	}
	
	// the avatar
	avatar->update();
	
	// all particles
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		(*it)->update();
	}
}

void StateLevel::render() {
	// background
	bg->render();
	
	// the avatar
	avatar->render();
	
	// all particles
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		(*it)->render();
	}
}

void StateLevel::reload() {
	clear();
	assemble();
}

void StateLevel::assemble() {
	assembleAvatar();
	
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
	}
}

void StateLevel::assembleAvatar() {
	Configuration conf = raw.getConfig("avatar");
	avatar = new Particle();
	avatar->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	avatar->speed = r2vec(conf.getReal("speedX"), conf.getReal("speedX"));
	avatar->setElasticity(conf.getReal("k"));
	avatar->setMass(conf.getReal("m"));
	avatar->charge = conf.getReal("q");
	
	// sprite
	avatar->sprite = sprite_avatar;
	((Circle*)avatar->getShape())->setRadius(avatar->sprite->rectW()/2);
}

Particle* StateLevel::assembleParticle(const Configuration& conf) {
	Particle* particle = new Particle();
	particle->pinned = true;
	particle->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	particle->setElasticity(conf.getReal("k"));
	particle->setMass(conf.getReal("m"));
	particle->charge = conf.getReal("q");
	
	// sprite
	if (avatar->charge < 0)
		avatar->sprite = sprite_negative;
	else if (avatar->charge == 0)
		avatar->sprite = sprite_neutral;
	else
		avatar->sprite = sprite_positive;
	((Circle*)particle->getShape())->setRadius(avatar->sprite->srcW()/2);
	
	return particle;
}

void StateLevel::clear() {
	// the avatar
	delete avatar;
	
	// all particles
	while (particles.size()) {
		delete particles.back();
		particles.pop_back();
	}
	
	// all interactions
	interactions.clear();
}
