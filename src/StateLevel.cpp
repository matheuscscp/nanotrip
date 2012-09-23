#include <sstream>

#include "StateLevel.hpp"

#include "Animation.hpp"
#include "Circle.hpp"
#include "InputManager.hpp"
#include "SDLBase.hpp"

#define LOSE_DELAY	4000

using namespace common;
using namespace lalge;

using std::list;

GAMESTATE_DEF(StateLevel)

StateLevel::UnstackArgs::UnstackArgs(int op) : op(op) {}

StateLevel::Args::Args(const std::string& levelname) : levelname(levelname) {}

StateLevel::StateLevel(ArgsBase* args) :
is_bg_init(false),
win_(false),
lose_(false),
max_abs_charge(1),
charge_cursor_position(640),
life(3),
border_top(0),
border_right(0),
border_bottom(0),
border_left(0)
{
	// screen box
	screen_box.position = r2vec(640, 360);
	screen_box.setWidth(1280);
	screen_box.setHeight(720);
	
	// background
	bg_grad = new Sprite("img/level/background.png");
	bg_nograd = new Sprite("img/level/background.png");
	bg_grad->render();
	bg_grad->setAlpha(0.3f);
	bg_nograd->setAlpha(0.15f);
	hud = new Sprite("img/level/hud.png");
	eatles = new Sprite("img/level/eatles.png");
	sprite_life = new Animation("img/level/life.png", 3, 1, 4, 1);
	
	// all sprites
	sprite_avatar = new Animation("img/level/avatar_positive.png", 0, 7, 1, 16);
	sprite_hole = new Sprite("img/level/blackhole.png");
	sprite_negative = new Sprite("img/level/negative.png");
	sprite_negative_anim = new Animation("img/level/negative_ssheet.png", 0, 20, 1, 16);
	sprite_neutral = new Sprite("img/level/neutral.png");
	sprite_positive = new Sprite("img/level/positive.png");
	sprite_positive_anim = new Animation("img/level/positive_ssheet.png", 0, 20, 1, 16);
	
	// all sounds
	sound_lose = new Audio("sfx/level/lose.wav");
	
	// configuration file
	raw.readTxt(RootPath::get("level/" + ((Args*)args)->levelname + ".conf"));
	
	delete args;
	
	// input hooks
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &StateLevel::handleKeyDown);
	InputManager::instance()->connect(InputManager::MOUSEMOTION, this, &StateLevel::handleMouseMotion);
	
	// texts
	text_press_space = new Text("ttf/Swiss721BlackRoundedBT.ttf", "Press space", 100, 0, SDLBase::getColor(255, 255, 255), Text::blended);
	text_time = new Text("ttf/Swiss721BlackRoundedBT.ttf", "0:00", 14, 0, SDLBase::getColor(255, 31, 77), Text::blended);
	text_you_lose = new Text("ttf/Swiss721BlackRoundedBT.ttf", "You lose", 100, 0, SDLBase::getColor(255, 255, 255), Text::blended);
	
	// charge changer
	charge_bar = new Sprite("img/level/charge_bar.png");
	charge_cursor = new Sprite("img/level/charge_cursor.png");
	
	// general config
	{
		Configuration general = raw.getConfig("general");
		
		// level time
		level_time = general.getInt("level_time");
		if (level_time < 5)
			level_time = 5;
		else if (level_time > 599)
			level_time = 599;
		timer.connect(Timer::DONE, this, &StateLevel::handleTimerDone);
		
		max_abs_charge = general.getReal("max_abs_charge");
		
		// borders
		if (general.getInt("border_top"))
			border_top = new Sprite("img/level/border_top_bottom.png");
		if (general.getInt("border_right"))
			border_right = new Sprite("img/level/border_right.png");
		if (general.getInt("border_bottom"))
			border_bottom = new Sprite("img/level/border_top_bottom.png");
		if (general.getInt("border_left"))
			border_left = new Sprite("img/level/border_left.png");
	}
	
	assemble();
}

StateLevel::~StateLevel() {
	clear();
	
	// all sprites
	delete bg_grad;
	delete bg_nograd;
	delete hud;
	delete eatles;
	delete sprite_life;
	delete sprite_avatar;
	delete sprite_hole;
	delete sprite_negative;
	delete sprite_negative_anim;
	delete sprite_neutral;
	delete sprite_positive;
	delete sprite_positive_anim;
	
	// all sounds
	delete sound_lose;
	
	// borders
	if (border_top)
		delete border_top;
	if (border_right)
		delete border_right;
	if (border_bottom)
		delete border_bottom;
	if (border_left)
		delete border_left;
	
	// texts
	delete text_press_space;
	delete text_time;
	delete text_you_lose;
	
	// charge cursor
	delete charge_bar;
	delete charge_cursor;
}

void StateLevel::handleUnstack(ArgsBase* args) {
	frozen_ = false;
	if (!args)
		return;
	
	int op = ((UnstackArgs*)args)->op;
	delete args;
	
	switch (op) {
	case UnstackArgs::TRYAGAIN:
		life = 3;
		((Animation*)sprite_life)->setFrame(life);
		lose_ = false;
		reload();
		break;
		
	case UnstackArgs::MAINMENU:
		throw new Change("StateMainMenu");
		break;
		
	default:
		break;
	}
}

void StateLevel::update() {
	// timer
	timer.update();
	
	// animations
	sprite_avatar->update();
	sprite_negative_anim->update();
	sprite_positive_anim->update();
	
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
	
	// handling possibilities to lose
	if (!lose_) {
		// update the time text only after the level starts
		if (!avatar->pinned)
			setTimeText(round(float(timer.time())/1000));
		
		// checking if the avatar is far away
		if (!screen_box.Shape::pointInside(avatar->getShape()->position))
			lose();
		
		// unpin particles
		if ((timer.time()) && (timer.time() <= 2000) && (bg == bg_grad)) {
			bg = bg_nograd;
			unpinParticles();
		}
	}
	// if the message was shown
	else if (stopwatch.time() >= LOSE_DELAY) {
		// if it was the last try
		if (life < 0) {
			frozen_ = true;
			throw new StackUp("StateGameOver");
		}
		
		lose_ = false;
		reload();
	}
}

void StateLevel::render() {
	// background
	bg->render();
	
	// all particles
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		(*it)->render();
	}
	
	// the hole
	hole->render();
	
	// the avatar
	avatar->render();
	
	// borders
	if (border_top)
		border_top->render();
	if (border_right)
		border_right->render(1250, 0);
	if (border_bottom)
		border_bottom->render(0, 700);
	if (border_left)
		border_left->render();
	
	// hud
	hud->render();
	eatles->render(45, 30);
	text_time->render(288, 54);
	sprite_life->render(236, 161);
	
	// charge changer
	charge_bar->render(0, 710);
	charge_cursor->render(charge_cursor_position, 715, true);
	
	// main message
	if ((lose_) && (stopwatch.time() <= LOSE_DELAY - 1000))
		text_you_lose->render(640, 360);
	else if ((avatar->pinned) && ((SDL_GetTicks()/600) % 2))
		text_press_space->render(640, 360);
}

void StateLevel::reload() {
	clear();
	assemble();
}

void StateLevel::assemble() {
	setTimeText(level_time);
	
	bg = bg_grad;
	
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
	//if (!is_bg_init)
		//bg->gradient(hole->getShape()->position.x(0), hole->getShape()->position.x(1), 200, 127, 127, 127, 0);
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
		particle->sprite = sprite_negative_anim;
		if (!is_bg_init)
			bg->gradient(particle->getShape()->position.x(0), particle->getShape()->position.x(1), 1500000*particle->charge*particle->charge, 255, 31, 77, 0);
	}
	else {
		particle->sprite = sprite_positive_anim;
		if (!is_bg_init)
			bg->gradient(particle->getShape()->position.x(0), particle->getShape()->position.x(1), 1500000*particle->charge*particle->charge, 51, 74, 144, 0);
	}
	((Circle*)particle->getShape())->setRadius(particle->sprite->rectW()/2);
	
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

void StateLevel::setTimeText(int seconds) {
	std::stringstream ss;
	int minutes = seconds/60;
	seconds -= (minutes*60);
	ss << minutes;
	ss << ":";
	if (seconds < 10)
		ss << "0";
	ss << seconds;
	text_time->setText(ss.str());
}

void StateLevel::handleKeyDown(const observer::Event& event, bool& stop) {
	switch (inputmanager_event.key.keysym.sym) {
	case SDLK_ESCAPE:
		frozen_ = true;
		throw new StackUp("StateMenu");
		break;
		
	case SDLK_SPACE:
		if (!avatar->pinned)
			return;
		avatar->pinned = false;
		timer.start(level_time*1000);
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

void StateLevel::handleTimerDone(const observer::Event& event, bool& stop) {
	if ((!lose_) && (!win_))
		lose();
}

void StateLevel::lose() {
	lose_ = true;
	
	life--;
	if (life >= 0)
		((Animation*)sprite_life)->setFrame(life);
	
	timer.cancel();
	
	sound_lose->play(1);
	
	stopwatch.start();
}

void StateLevel::gameOver() {
	//TODO
}

void StateLevel::unpinParticles() {
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		(*it)->pinned = false;
		
		// sprite
		if ((*it)->charge < 0)
			(*it)->sprite = sprite_negative;
		else if ((*it)->charge > 0)
			(*it)->sprite = sprite_positive;
	}
}
