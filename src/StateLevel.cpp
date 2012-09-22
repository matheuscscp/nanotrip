#include "StateLevel.hpp"

#include "Animation.hpp"
#include "Circle.hpp"
#include "InputManager.hpp"
#include "SDLBase.hpp"

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
	sprite_avatar = new Animation("img/level/avatar_positive.png", 0, 7, 1, 16);
	sprite_negative = new Sprite("img/level/negative.png");
	sprite_neutral = new Sprite("img/level/neutral.png");
	sprite_positive = new Sprite("img/level/positive.png");
	
	// configuration file
	raw.readTxt(RootPath::get("level/" + ((Args*)args)->levelname + ".conf"));
	
	assemble();
	
	delete args;
	
	// input hooks
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &StateLevel::handleKeyDown);
	
	// press space to start
	press_space = new Text("ttf/Swiss721BlackRoundedBT.ttf", "Press space", 100, 0, SDLBase::getColor(255, 255, 255), Text::blended, SDLBase::getColor(0, 0, 0));
}

StateLevel::~StateLevel() {
	clear();
	
	// all sprites
	delete bg;
	delete sprite_avatar;
	delete sprite_negative;
	delete sprite_neutral;
	delete sprite_positive;
	
	// press space to start
	delete press_space;
}

void StateLevel::handleUnstack(ArgsBase* args) {
	/*switch (((Args*)args)->unstack_op) {
	case x:
		x;
		break;
		
	default:
		break;
	}*/
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
	
	// press space to start
	if ((avatar->pinned) && ((SDL_GetTicks()/600) % 2))
		press_space->render(640, 360);
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
	avatar->pinned = true;
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
	if (particle->charge == 0)
		particle->sprite = sprite_neutral;
	else if (particle->charge < 0) {
		particle->sprite = sprite_negative;
		bg->gradient(particle->getShape()->position.x(0), particle->getShape()->position.x(1), 1500000*particle->charge*particle->charge, 255, 31, 77, 0);
	}
	else {
		particle->sprite = sprite_positive;
		bg->gradient(particle->getShape()->position.x(0), particle->getShape()->position.x(1), 1500000*particle->charge*particle->charge, 51, 74, 144, 0);
	}
	((Circle*)particle->getShape())->setRadius(particle->sprite->srcW()/2);
	
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

void StateLevel::handleKeyDown(const observer::Event& event, bool& stop) {
	switch (inputmanager_event.key.keysym.sym) {
	case SDLK_ESCAPE:
		frozen_ = (!frozen_);
		//throw new StackUp("StatePause", 0);
		break;
		
	case SDLK_SPACE:
		avatar->pinned = false;
		break;
		
	case 'r':
		reload();
		break;
		
	default:
		break;
	}
}
