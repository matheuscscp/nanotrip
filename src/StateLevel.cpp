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

StateLevel::StateLevel(ArgsBase* args) :
is_bg_init(false),
max_abs_charge(1),
charge_cursor_position(640),
life(3)
{
	// background
	bg = new Sprite("img/level/background.png");
	bg->render();
	bg->setAlpha(0.3f);
	hud = new Sprite("img/level/hud.png");
	eatles = new Sprite("img/level/eatles.png");
	
	sprite_life[0].load("img/level/0life.png");
	sprite_life[1].load("img/level/1life.png");
	sprite_life[2].load("img/level/2life.png");
	sprite_life[3].load("img/level/3life.png");
	
	// all sprites
	sprite_avatar = new Animation("img/level/avatar_positive.png", 0, 7, 1, 16);
	sprite_hole = new Sprite("img/level/hole.png");
	sprite_negative = new Sprite("img/level/negative.png");
	sprite_neutral = new Sprite("img/level/neutral.png");
	sprite_positive = new Sprite("img/level/positive.png");
	
	// configuration file
	raw.readTxt(RootPath::get("level/" + ((Args*)args)->levelname + ".conf"));
	
	assemble();
	
	delete args;
	
	// input hooks
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &StateLevel::handleKeyDown);
	InputManager::instance()->connect(InputManager::MOUSEMOTION, this, &StateLevel::handleMouseMotion);
	
	// press space to start
	press_space = new Text("ttf/Swiss721BlackRoundedBT.ttf", "Press space", 100, 0, SDLBase::getColor(255, 255, 255), Text::blended, SDLBase::getColor(0, 0, 0));
	
	// charge changer
	max_abs_charge = raw.getConfig("general").getReal("max_abs_charge");
	charge_bar = new Sprite("img/level/charge_bar.png");
	charge_cursor = new Sprite("img/level/charge_cursor.png");
}

StateLevel::~StateLevel() {
	clear();
	
	// all sprites
	delete bg;
	delete hud;
	delete eatles;
	delete sprite_avatar;
	delete sprite_hole;
	delete sprite_negative;
	delete sprite_neutral;
	delete sprite_positive;
	
	// press space to start
	delete press_space;
	
	// charge cursor
	delete charge_bar;
	delete charge_cursor;
}

void StateLevel::handleUnstack(ArgsBase* args) {
	frozen_ = false;
	if (args) {
		delete args;
		throw new Change("StateMainMenu");
	}
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
	hud->render();
	eatles->render(45, 30);
	sprite_life[life].render(236, 161);
	
	// all particles
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		(*it)->render();
	}
	
	// the hole
	hole->render();
	
	// the avatar
	avatar->render();
	
	// charge changer
	charge_bar->render(0, 710);
	charge_cursor->render(charge_cursor_position, 715, true);
	
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
	
	assembleHole();
	
	// avatar-hole force interaction
	interactions.push_back(Interaction(avatar, hole, (Interaction::callback)&Particle::addParticleFieldForces, true));
	
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
	
	is_bg_init = true;
}

void StateLevel::assembleAvatar() {
	Configuration conf = raw.getConfig("avatar");
	avatar = new Particle();
	avatar->pinned = true;
	avatar->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	avatar->speed = r2vec(conf.getReal("speedX"), conf.getReal("speedY"));
	avatar->setElasticity(conf.getReal("k"));
	avatar->setMass(conf.getReal("m"));
	avatar->charge = conf.getReal("q");
	
	// sprite
	avatar->sprite = sprite_avatar;
	((Circle*)avatar->getShape())->setRadius(avatar->sprite->rectW()/2);
}

void StateLevel::assembleHole() {
	Configuration conf = raw.getConfig("hole");
	hole = new Particle();
	hole->pinned = true;
	hole->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	hole->setElasticity(conf.getReal("k"));
	hole->setMass(conf.getReal("m"));
	hole->charge = conf.getReal("q");
	
	// sprite
	hole->sprite = sprite_hole;
	if (!is_bg_init)
		bg->gradient(hole->getShape()->position.x(0), hole->getShape()->position.x(1), 100, 127, 127, 127, 0);
	((Circle*)hole->getShape())->setRadius(hole->sprite->rectW()/2);
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
		if (!is_bg_init)
			bg->gradient(particle->getShape()->position.x(0), particle->getShape()->position.x(1), 1500000*particle->charge*particle->charge, 255, 31, 77, 0);
	}
	else {
		particle->sprite = sprite_positive;
		if (!is_bg_init)
			bg->gradient(particle->getShape()->position.x(0), particle->getShape()->position.x(1), 1500000*particle->charge*particle->charge, 51, 74, 144, 0);
	}
	((Circle*)particle->getShape())->setRadius(particle->sprite->srcW()/2);
	
	return particle;
}

void StateLevel::clear() {
	// the avatar
	delete avatar;
	
	// the hole
	delete hole;
	
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
		frozen_ = true;
		throw new StackUp("StateMenu");
		break;
		
	case SDLK_SPACE:
		avatar->pinned = false;
		break;
		
	default:
		break;
	}
}

void StateLevel::handleMouseMotion(const observer::Event& event, bool& stop) {
	// handling avatar charge variables
	charge_cursor_position = InputManager::instance()->mouseX();
	avatar->charge = max_abs_charge*(charge_cursor_position - 640)/640;
}
